#include "../utils.h"
#include <algorithm>

using Num = int64_t;
using Board = Grid<>;

static Num
solve(Board b, int tolerance = 4, int maxRange = 1) {
  std::vector<Pos> changed;
  do {
    changed.clear();
    for (auto [x, y, v] : b.iter()) {
      if (b[x, y] == '.') {
        continue;
      }
      int occ = 0;
      for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
          if (dx == 0 && dy == 0) {
            continue;
          }
          for (int r = 1; r <= maxRange; ++r) {
            auto v = b[x + dy * r, y + dx * r];
            if (v != '.') {
              occ += v == '#';
              break;
            }
          }
        }
      }
      if (occ == 0 && b[x, y] == 'L') {
        changed.emplace_back(x, y);
      } else if (occ >= tolerance && b[x, y] == '#') {
        changed.emplace_back(x, y);
      }
    }
    for (auto p : changed) {
      b[p] = b[p] == 'L' ? '#' : 'L';
    }
  } while (!changed.empty());
  return ranges::count(b, '#');
}

/* ------------------------------------------------------------------------ */

int
main() {
  Board b = Board::read();

  println("1: {}", solve(b));
  println("2: {}", solve(b, 5, std::numeric_limits<int>::max()));

  return 0;
}
