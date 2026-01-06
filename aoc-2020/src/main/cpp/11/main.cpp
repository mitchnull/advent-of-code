#include "../utils.h"
#include <algorithm>

using Num = int64_t;
using Board = Grid<>;

static Num
solve1(Board b) {
  std::vector<Pos> changed;
  do {
    changed.clear();
    for (auto [x, y, v] : b.iter()) {
      if (b[x, y] == '.') {
        continue;
      }
      int occ = 0;
      for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
          occ += (i != 0 || j != 0) && b[x + j, y + i] == '#';
        }
      }
      if (occ == 0 && b[x, y] == 'L') {
        changed.emplace_back(x, y);
      } else if (occ >= 4 && b[x, y] == '#') {
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

  println("1: {}", solve1(b));

  return 0;
}
