#include "../utils.h"
#include <algorithm>
#include <numeric>

using Num = int64_t;

struct Pos3d {
  int x, y, z;

  auto friend operator<=>(const Pos3d &a, const Pos3d &b) = default;
};

template <>
struct std::hash<Pos3d> {
  std::size_t operator()(const Pos3d &p) const { return hashCombine(hashCombine(p.x, p.y), p.z); }
};

template <>
struct std::formatter<Pos3d> {
  constexpr auto parse(std::format_parse_context &ctx) { return ctx.begin(); }

  auto format(const Pos3d &pos, std::format_context &ctx) const {
    return std::format_to(ctx.out(), "{{{}, {}, {}}}", pos.x, pos.y, pos.z);
  }
};

struct Brick {
  Pos3d b, e;
};

template <>
struct std::formatter<Brick> {
  constexpr auto parse(std::format_parse_context &ctx) { return ctx.begin(); }

  auto format(const Brick &b, std::format_context &ctx) const { return std::format_to(ctx.out(), "{}~{}", b.b, b.e); }
};

using Bricks = std::vector<Brick>;

/* ------------------------------------------------------------------------ */

static bool
isOverlapping(const Brick &b, const Brick &t) {
  return t.e.x >= b.b.x && b.e.x >= t.b.x && t.e.y >= b.b.y && b.e.y >= t.b.y;
}

static void
dropDown(Bricks &bricks, int i) {
  int level = 0;
  for (int j = i - 1; j >= 0; --j) {
    if (isOverlapping(bricks[i], bricks[j])) {
      level = std::max(level, bricks[j].e.z);
    }
  }
  bricks[i].e.z -= bricks[i].b.z - (level + 1);
  bricks[i].b.z = level + 1;
}

static int
supporters(const Bricks &bricks, const std::vector<bool> &disintegrated, int i) {
  int res = 0;
  for (int j = i - 1; j >= 0; --j) {
    res += !disintegrated[j] && (bricks[j].e.z == bricks[i].b.z - 1) && isOverlapping(bricks[i], bricks[j]);
  }
  return res;
}

static bool
isFree(const Bricks &bricks, const std::vector<int> &supps, int i) {
  for (int j = i + 1; j < bricks.size(); ++j) {
    if (supps[j] == 1 && bricks[j].b.z == bricks[i].e.z + 1 && isOverlapping(bricks[i], bricks[j])) {
      return false;
    }
  }
  return true;
}

static Num
disintegrate(const Bricks &bricks, int i) {
  Num res = 0;
  auto disintegrated = std::vector<bool>(bricks.size());
  disintegrated[i] = true;

  for (int j = i + 1; j < bricks.size(); ++j) {
    if (bricks[j].b.z > 1 && supporters(bricks, disintegrated, j) == 0) {
      disintegrated[j] = true;
      ++res;
    }
  }
  return res;
}

static Num
solve2(const Bricks &bricks, const std::vector<int> &supps) {
  Num res = 0;
  for (int i = 0; i < bricks.size(); ++i) {
    res += disintegrate(bricks, i);
  }
  return res;
}

/* ------------------------------------------------------------------------ */

int
main() {
  char ch;
  Bricks bricks;
  int x1, y1, z1, x2, y2, z2;
  while (std::cin >> x1 >> ch >> y1 >> ch >> z1 >> ch >> x2 >> ch >> y2 >> ch >> z2) {
    bricks.emplace_back(Pos3d{std::min(x1, x2), std::min(y1, y2), std::min(z1, z2)},
        Pos3d{std::max(x1, x2), std::max(y1, y2), std::max(z1, z2)});
  }
  ranges::sort(bricks, [](const auto &a, const auto &b) { return a.b.z < b.b.z; });
  // for (const auto &b : bricks) {
  //   println("@@@0 {}~{}", b.b, b.e);
  // }

  for (int i = 0; i < bricks.size(); ++i) {
    dropDown(bricks, i);
  }

  // for (const auto &b : bricks) {
  //   println("@@@1 {}~{}", b.b, b.e);
  // }
  auto none = std::vector<bool>(bricks.size(), false);
  auto supps = views::iota(0UZ, bricks.size()) | views::transform([&](auto i) { return supporters(bricks, none, i); }) |
      ranges::to<std::vector<int>>();
  Num res1 = ranges::count(
      views::iota(0UZ, bricks.size()) | views::transform([&](auto i) { return isFree(bricks, supps, i); }), true);

  Num res2 = solve2(bricks, supps);

  println("1: {}", res1);
  println("2: {}", res2);

  return 0;
}
