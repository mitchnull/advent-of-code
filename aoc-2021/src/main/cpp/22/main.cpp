#include "../utils.h"
#include <algorithm>
#include <regex>
#include <valarray>

constexpr const int N = 3;
using Num = int64_t;
using V = std::valarray<Num>;

struct Cube {
  V bl, tr;
  bool on = false;
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
  for (int i = 0; i < N; ++i) {
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
    for (int i = 0; i < N; ++i) {
      if (cc.bl[i] < c.bl[i]) {
        // println("@@@ split left: {}:", cc);
        Cube lc = cc;
        lc.tr[i] = c.bl[i] - 1;
        cc.bl[i] = c.bl[i];
        // println("@@@ after split left ({}): {} + {}:", i, lc, cc);
        next.push_back(std::move(lc));
      }
      if (cc.tr[i] > c.tr[i]) {
        // println("@@@ split right: {}:", cc);
        Cube rc = cc;
        rc.bl[i] = c.tr[i] + 1;
        cc.tr[i] = c.tr[i];
        // println("@@@ after split right ({}): {} + {}:", i, cc, rc);
        next.push_back(std::move(rc));
      }
    }
  }
  return next;
}

static bool
isBootArea(const Cube &c) {
  for (auto w : {&Cube::bl, &Cube::tr}) {
    for (int i = 0; i < N; ++i) {
      if ((c.*w)[i] < -50 || (c.*w)[i] > 50) {
        return false;
      }
    }
  }
  return true;
}

static void
debug(const Cubes &core) {
  const int MIN = 0, MAX = 13;
  for (int i = MIN; i <= MAX; ++i) {
    for (int j = MIN; j <= MAX; ++j) {
      for (int k = MIN; k <= MAX; ++k) {
        for (const auto &c : core) {
          if (isColliding(c, Cube{{i, j, k}, {i, j, k}, false})) {
            println("- {},{},{}", i, j, k);
          }
        }
      }
    }
  }
}

static std::pair<Num, Num>
solve(const Cubes &input) {
  Cubes core;
  for (const auto &c : input) {
    // println("@@@ splitting by {}, {}", c, core.size());
    core = split(core, c);
    if (c.on) {
      core.push_back(c);
    }
    // println("@@@ after splitting: {}", core);
    // debug(core); // @@@
  }
  Num res1 = 0, res2 = 0;
  for (const auto &c : core) {
    V d = (c.tr - c.bl) + 1;
    Num v = ranges::fold_left(d, Num{1}, std::multiplies<>{});
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
    auto it = std::sregex_iterator(line.begin(), line.end(), re);
    Num x1 = std::stoi(it++->str());
    Num x2 = std::stoi(it++->str());
    Num y1 = std::stoi(it++->str());
    Num y2 = std::stoi(it++->str());
    Num z1 = std::stoi(it++->str());
    Num z2 = std::stoi(it++->str());
    cubes.emplace_back(V{std::min(x1, x2), std::min(y1, y2), std::min(z1, z2)},
        V{std::max(x1, x2), std::max(y1, y2), std::max(z1, z2)},
        line[1] == 'n');
  }
  auto [res1, res2] = solve(cubes);
  println("1: {}", res1);
  println("2: {}", res2);

  return 0;
}
