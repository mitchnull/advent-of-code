#include "../utils.h"

using Num = int64_t;
using Board = Grid<>;

/* ------------------------------------------------------------------------ */

static Num
flash(Board &b, int x, int y) {
  Num res = 1;
  for (int yy = y - 1; yy <= y + 1; ++yy) {
    for (int xx = x - 1; xx <= x + 1; ++xx) {
      if (b[xx, yy]++ == '9') {
        res += flash(b, xx, yy);
      }
    }
  }
  return res;
}

static Num
solve1(Board b, Num iters) {
  Num res = 0;
  while (iters-- > 0) {
    for (int y = 0; y < b.h(); ++y) {
      for (int x = 0; x < b.w(); ++x) {
        if (b[x, y]++ == '9') {
          res += flash(b, x, y);
        }
      }
    }
    std::transform(b.begin(), b.end(), b.begin(), [](auto o) { return (o > '9') ? '0' : o; });
  }
  return res;
}

static Num
solve2(Board b) {
  for (int i = 1; ; ++i) {
    for (int y = 0; y < b.h(); ++y) {
      for (int x = 0; x < b.w(); ++x) {
        if (b[x, y]++ == '9') {
          flash(b, x, y);
        }
      }
    }
    if (std::find_if(b.begin(), b.end(), [](auto o) { return o <= '9'; }) == b.end()) {
      return i;
    }
    std::transform(b.begin(), b.end(), b.begin(), [](auto o) { return (o > '9') ? '0' : o; });
  }
  return 0;
}

/* ------------------------------------------------------------------------ */

int
main() {
  Board board = Board::read();

  println("1: {}", solve1(board, 100));
  println("1: {}", solve2(board));

  return 0;
}
