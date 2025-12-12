#include "../utils.h"
#include <sstream>
#include <algorithm>
#include <deque>
#include <bitset>
#include <valarray>
#include <numeric>

using Lights = std::bitset<16>;
using V = std::vector<int>;
using VA = std::valarray<int>;
using Mat = std::vector<VA>;

template <>
struct std::formatter<Mat> {
  bool pretty = false;
  constexpr auto parse(std::format_parse_context& ctx) {
    auto it = ctx.begin();
    if (it == ctx.end()) {
        return it;
    }
    if (*it == '#') {
        pretty = true;
        ++it;
    }
    if (it != ctx.end() && *it != '}') {
      throw std::format_error("invalid format args");
    }
    return it;
  }

  auto format(const Mat& m, std::format_context& ctx) const {
    if (pretty) {
      std::format_to(ctx.out(), "[\n");
      for (auto r : m) {
        std::format_to(ctx.out(), "{}\n", r);
      }
      std::format_to(ctx.out(), "]");
    } else {
      std::format_to(ctx.out(), "{}", m);
    }
    return ctx.out();
  }
};

struct Machine {
  Lights lights;
  std::vector<Lights> wirings;
  V jolts;
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

static void
degauss(Mat& g) {
  for (int i = 0; i < g.size(); ++i) {
    if (g[i][i] == 0) {
      for (int ii = i + 1; ii < g.size(); ++ii) {
        if (g[ii][i] != 0) {
          std::swap(g[i], g[ii]);
          println("@@@ swap: {}, {}, {:#}", i, ii, g);
          break;
        }
      }
    }
    if (g[i][i] == 0) {
      g.erase(g.begin() + i, g.end());
      println("@@@ prune: {}, {:#}", i, g);
    } else {
      for (int j = i + 1; j < g.size(); ++j) {
        if (g[j][i] != 0) {
          int lcm = std::lcm(g[j][i], g[i][i]);
          println("@@@ i: {}, j: {}, lcm: {}, mul: {}, div: {}", i, j, lcm, (lcm / g[j][i]), (lcm / g[i][i]));
          g[j] = g[j] * (lcm / g[j][i]) - g[i] * (lcm / g[i][i]);
          println("@@@ after {:#}", g);
        }
      }
    }
  }
  println("@@@ degauss: {:#}", g);
}

static bool
inc(VA& v, int n, int max) {
  for (int i = v.size() - 1; i >= n; --i) {
    if (++v[i] <= max) {
      return true;
    }
    v[i] = 0;
  }
  return false;
}

static int
check(const Mat& g, VA v) {
  println("@@@ checking: {}", v);
  for (int i = g.size() - 1; i >= 0; --i) {
    auto c = g[i][i];
    if (c == 0) {
      continue;
    }
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
  println("@@@ checked: {}: {}", v, v.sum());
  return v.sum();
}

static int
solve2(const Machine& m) {
  int max = 0;
  Mat g;
  for (int i = 0; i < m.jolts.size(); ++i) {
    auto v = VA(0, m.wirings.size() + 1);
    for (int j = 0; j < m.wirings.size(); ++j) {
      if (m.wirings[j][i]) {
        v[j] = 1;
      }
    }
    v[v.size() - 1] = m.jolts[i];
    max = std::max(max, m.jolts[i]);
    g.push_back(std::move(v));
  }
  println("@@@ {:#}", g);
  degauss(g);
  auto v = VA(0, m.wirings.size());
  auto res = std::numeric_limits<int>::max();
  do {
    res = std::min(res, check(g, v));
  } while (inc(v, g.size(), max));
  println("@@@ res: {:#}", res);
  return res;
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
