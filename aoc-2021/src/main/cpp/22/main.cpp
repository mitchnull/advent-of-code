#include "../utils.h"
#include <numeric>
#include <regex>

using Num = int64_t;
using V = std::vector<Num>;

struct Cube {
  V bl, tr;
  bool on;
};
using Cubes = std::vector<Cube>;

template <>
struct std::formatter<Cube> {
  template <typename FormatContext>
  constexpr auto parse(FormatContext &ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  constexpr auto format(const Cube &c, FormatContext &ctx) const {
    return format_to(ctx.out(), "{{{}-{}}}({})", c.bl, c.tr, c.on);
  }
};

static bool
isColliding(const Cube &a, const Cube &b) {
  for (int i = 0; i < a.bl.size(); ++i) {
    if (b.tr[i] < a.bl[i] || a.tr[i] < b.bl[i]) {
      return false;
    }
  }
  return true;
}

static Cubes
split(const Cubes &core, Cube c) {
  Cubes next;
  for (auto cc : core) {
    if (!isColliding(c, cc)) {
      next.push_back(std::move(cc));
      continue;
    }
    for (int i = 0; i < c.bl.size(); ++i) {
      if (cc.bl[i] < c.bl[i]) {
        Cube lc = cc;
        lc.tr[i] = c.bl[i] - 1;
        cc.bl[i] = c.bl[i];
        next.push_back(std::move(lc));
      }
      if (cc.tr[i] > c.tr[i]) {
        Cube rc = cc;
        rc.bl[i] = c.tr[i] + 1;
        cc.tr[i] = c.tr[i];
        next.push_back(std::move(rc));
      }
    }
  }
  return next;
}

static bool
isBootArea(const Cube &c) {
  for (auto w : {&Cube::bl, &Cube::tr}) {
    for (auto v : c.*w) {
      if (v < -50 || v > 50) {
        return false;
      }
    }
  }
  return true;
}

static std::pair<Num, Num>
solve(const Cubes &input) {
  Cubes core;
  for (const auto &c : input) {
    core = split(core, c);
    if (c.on) {
      core.push_back(c);
    }
  }
  Num res1 = 0, res2 = 0;
  for (const auto &c : core) {
    Num v = std::inner_product(
        begin(c.bl), end(c.bl), begin(c.tr), Num{1}, std::multiplies<>(), [](auto a, auto b) { return b - a + 1; });
    if (isBootArea(c)) {
      res1 += v;
    }
    res2 += v;
  }
  return {res1, res2};
}

/* ------------------------------------------------------------------------ */

int
main() {
  // on x=11..13,y=11..13,z=11..13
  // off x=9..11,y=9..11,z=9..11
  Cubes cubes;
  string line;
  std::regex re("([0-9-]+)");
  while (std::getline(std::cin, line)) {
    std::vector<Num> bl, tr;
    for (auto it = std::sregex_iterator(line.begin(), line.end(), re); it != std::sregex_iterator();) {
      bl.push_back(std::stoi(it++->str()));
      tr.push_back(std::stoi(it++->str()));
    }
    cubes.emplace_back(bl, tr, line[1] == 'n');
  }
  auto [res1, res2] = solve(cubes);
  println("1: {}", res1);
  println("2: {}", res2);

  return 0;
}
