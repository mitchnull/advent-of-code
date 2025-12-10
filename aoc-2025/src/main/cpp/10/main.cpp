#include "../utils.h"
#include <sstream>
#include <algorithm>
#include <deque>
#include <unordered_set>

using Lights = std::bitset<16>;
using V = std::vector<int>;
struct Machine {
  Lights lights;
  std::vector<Lights> wirings;
  V jolts;
};

template <>
struct std::hash<V> {
  std::size_t operator()(const V& jolts) const {
    std::size_t h = 11;
    for (const auto& v: jolts) {
      h = hashCombine(h, v);
    }
    return h;
  }
};

static int
solve1(const Machine& m) {
  auto q = std::deque<std::pair<Lights, int>>{{}};
  while (!q.empty()) {
    auto [lights, step] = q.front();
    q.pop_front();
    if (lights == m.lights) {
      return step;
    }
    for (auto w : m.wirings) {
      q.emplace_back(lights ^ w, step + 1);
    }
  }
  return -1;
}

static V
charge(V jolts, Lights w) {
  for (int i = 0; i < jolts.size(); ++i) {
    if (w[i]) {
      ++jolts[i];
    }
  }
  return jolts;
}

static bool
isOvercharged(const Machine& m, const V& jolts) {
  for (int i = 0; i < jolts.size(); ++i) {
    if (jolts[i] > m.jolts[i]) {
      return true;
    }
  }
  return false;
}

static int
solve2(const Machine& m) {
  println("@@@ checking: m: {}", m.jolts);
  auto wirings = std::vector<Lights>{};
  auto ways = std::unordered_set<V>{V(m.jolts.size(), 0)};
  for (int res = 1; ; ++res) {
    auto next = std::unordered_set<V>();
    for (auto pj : ways) {
      for (auto w : m.wirings) {
        auto jolts = charge(pj, w);
        if (jolts == m.jolts) {
          return res;
        }
        if (!isOvercharged(m, jolts)) {
          next.insert(jolts);
        }
      }
    }
    std::swap(ways, next);
  }
  return -1;
}

/* ------------------------------------------------------------------------ */

int
main() {
  std::string str;
  // [.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}
  std::vector<Machine> machines;
  while (std::getline(std::cin, str)) {
    std::istringstream line{str};
    line >> str;
    Lights lights{};
    for (int i = 1; i < str.size() - 1; ++i) {
      if (str[i] == '#') {
        lights[i - 1] = true;
      }
    }
    std::vector<Lights> wirings;
    V jolts;
    while (line >> str) {
      std::istringstream ss{str};
      char ch;
      int button;
      ss >> ch;
      if (ch == '(') {
        Lights wiring{};
        while (ss >> button >> ch) {
          wiring[button] = true;
        }
        wirings.push_back(wiring);
      } else {
        int jolt;
        while (ss >> jolt >> ch) {
          jolts.push_back(jolt);
        }
      }
    }
    machines.emplace_back(lights, wirings, jolts);
  }

  int res1 = ranges::fold_left(machines | views::transform([](const auto& m) { return solve1(m); }), 0, std::plus<>());
  int res2 = ranges::fold_left(machines | views::transform([](const auto& m) { return solve2(m); }), 0, std::plus<>());

  println("1: {}", res1);
  println("2: {}", res2);

  return 0;
}
