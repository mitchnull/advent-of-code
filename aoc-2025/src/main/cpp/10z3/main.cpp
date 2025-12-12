#include "../utils.h"
#include <sstream>
#include <algorithm>
#include <deque>
#include <bitset>
#include <z3++.h>

using Lights = std::bitset<16>;
using V = std::vector<std::int16_t>;

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

static int
solve2(const Machine& m) {
  z3::context ctx;
  z3::optimize opt{ctx};
  z3::expr_vector v{ctx};

  for (int i = 0; i < m.wirings.size(); ++i) {
    z3::expr xi = ctx.int_const(std::format("x{}", i).c_str());
    v.push_back(xi);
    opt.add(xi >= 0);
  }
  z3::expr z = ctx.int_const("z");
  opt.add(z == z3::sum(v));
  for (int j = 0; j < m.jolts.size(); ++j) {
    z3::expr_vector vv{ctx};
    for (int i = 0; i < m.wirings.size(); ++i) {
      if (m.wirings[i][j]) {
        vv.push_back(v[i]);
      }
    }
    opt.add(m.jolts[j] == z3::sum(vv));
  }
  auto h = opt.minimize(z);
  opt.check();

  // <<<
  std::cout << "@@@ x: [";
  std::string sep = "";
  for (int i = 0; i < v.size(); ++i) {
    std::cout << sep << opt.get_model().eval(v[i]);
    sep = ", ";
  }
  std::cout << "]\n";
  // >>>
  return opt.lower(h).as_int64();
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
