#include "../utils.h"
#include <sstream>
#include <algorithm>
#include <deque>

using Lights = std::bitset<16>;
struct Machine {
  Lights lights;
  std::vector<Lights> wirings;
  std::vector<int> jolts;
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
    std::vector<int> jolts;
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

  println("1: {}", res1);
  println("2: {}", 0);

  return 0;
}
