#include "../utils.h"
#include <algorithm>
#include <bitset>
#include <deque>
#include <numeric>
#include <sstream>
#include <unordered_set>
#include <valarray>

using Lights = std::bitset<16>;
using V = std::vector<int>;
using VA = std::valarray<int>;
using Mat = std::vector<VA>;

struct Machine {
  Lights lights;
  std::vector<Lights> wirings;
  V jolts;
};

/* ------------------------------------------------------------------------ */

static int
solve1(const Machine &m) {
  auto visited = std::unordered_set<Lights>{Lights{}};
  auto q = std::deque<std::pair<Lights, int>>{{}};
  while (!q.empty()) {
    auto [lights, step] = q.front();
    q.pop_front();
    if (lights == m.lights) {
      return step;
    }
    for (auto w : m.wirings) {
      if (visited.insert(lights ^ w).second) {
        q.emplace_back(lights ^ w, step + 1);
      }
    }
  }
  return -1;
}

/* ------------------------------------------------------------------------ */

static void
degauss(Mat &g, VA &limits) {
  for (int i = 0; i < g.size();) {
    if (g[i][i] == 0) {
      for (int k = i + 1; k < g.front().size() - 1; ++k) {
        if (g[i][k] != 0) {
          for (int j = 0; j < g.size(); ++j) {
            std::swap(g[j][i], g[j][k]);
            std::swap(limits[i], limits[k]);
          }
          break;
        }
      }
    }
    if (g[i][i] == 0) {
      g.erase(g.begin() + i);
      continue;
    }
    for (int j = i + 1; j < g.size(); ++j) {
      if (g[j][i] != 0) {
        int lcm = std::lcm(g[j][i], g[i][i]);
        g[j] = g[j] * (lcm / g[j][i]) - g[i] * (lcm / g[i][i]);
      }
    }
    ++i;
  }
}

static bool
inc(VA &v, int n, const VA &limits) {
  for (int i = v.size() - 1; i >= n; --i) {
    if (++v[i] <= limits[i]) {
      return true;
    }
    v[i] = 0;
  }
  return false;
}

static int
check(const Mat &g, VA v) {
  for (int i = g.size() - 1; i >= 0; --i) {
    auto c = g[i][i];
    int xv = 0;
    for (int j = i + 1; j < v.size(); ++j) {
      xv += g[i][j] * v[j];
    }
    xv = g[i][v.size()] - xv;
    v[i] = xv / c;
    if (v[i] < 0 || xv % c != 0) {
      return std::numeric_limits<int>::max();
    }
  }
  return v.sum();
}

static int
solve2(const Machine &m) {
  VA limits = VA(std::numeric_limits<int>::max(), m.wirings.size());
  Mat g;
  for (int i = 0; i < m.jolts.size(); ++i) {
    auto v = VA(0, m.wirings.size() + 1);
    for (int j = 0; j < m.wirings.size(); ++j) {
      if (m.wirings[j][i]) {
        v[j] = 1;
        limits[j] = std::min(limits[j], m.jolts[i]);
      }
    }
    v[v.size() - 1] = m.jolts[i];
    g.push_back(std::move(v));
  }
  degauss(g, limits);
  auto v = VA(0, m.wirings.size());
  auto res = std::numeric_limits<int>::max();
  do {
    res = std::min(res, check(g, v));
  } while (inc(v, g.size(), limits));
  return res;
}

/* ------------------------------------------------------------------------ */

int
main() {
  std::string str;
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

  int res1 = ranges::fold_left(machines | views::transform([](const auto &m) { return solve1(m); }), 0, std::plus<>());
  int res2 = ranges::fold_left(machines | views::transform([](const auto &m) { return solve2(m); }), 0, std::plus<>());

  println("1: {}", res1);
  println("2: {}", res2);

  return 0;
}
