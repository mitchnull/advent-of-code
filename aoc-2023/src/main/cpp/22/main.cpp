#include "../utils.h"
#include <algorithm>

using Num = int64_t;

struct Pos3d {
  int x, y, z;
};

struct Brick {
  Pos3d b, e;
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
supporters(const Bricks &bricks, int i) {
  int res = 0;
  for (int j = i - 1; j >= 0; --j) {
    res += (bricks[j].e.z == bricks[i].b.z - 1) && isOverlapping(bricks[i], bricks[j]);
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
disintegrate(Bricks bricks, int i) {
  Num res = 0;

  bricks.erase(bricks.begin() + i);

  for (int j = i; j < bricks.size();) {
    if (bricks[j].b.z > 1 && supporters(bricks, j) == 0) {
      bricks.erase(bricks.begin() + j);
      ++res;
    } else {
      ++j;
    }
  }
  return res;
}

static Num
solve2(const Bricks &bricks) {
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

  for (int i = 0; i < bricks.size(); ++i) {
    dropDown(bricks, i);
  }

  auto supps = views::iota(0UZ, bricks.size()) | views::transform([&](auto i) { return supporters(bricks, i); }) |
      ranges::to<std::vector<int>>();
  Num res1 = ranges::count(
      views::iota(0UZ, bricks.size()) | views::transform([&](auto i) { return isFree(bricks, supps, i); }), true);

  Num res2 = solve2(bricks);

  println("1: {}", res1);
  println("2: {}", res2);

  return 0;
}
