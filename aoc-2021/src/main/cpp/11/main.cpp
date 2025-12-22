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

static std::pair<Num, Num>
solve(Board b, int iters) {
  Num res1 = 0, res2 = 0;
  for (int i = 1; i <= iters || res2 == 0; ++i) {
    for (int y = 0; y < b.h(); ++y) {
      for (int x = 0; x < b.w(); ++x) {
        if (b[x, y]++ == '9') {
          if (i <= iters) {
            res1 += flash(b, x, y);
          } else {
            flash(b, x, y);
          }
        }
      }
    }
    if (res2 == 0 && std::find_if(b.begin(), b.end(), [](auto o) { return o <= '9'; }) == b.end()) {
      res2 = i;
    }
    std::transform(b.begin(), b.end(), b.begin(), [](auto o) { return (o > '9') ? '0' : o; });
  }
  return {res1, res2};
}

/* ------------------------------------------------------------------------ */

int
main() {
  Board board = Board::read();

  auto [res1, res2] = solve(board, 100);
  println("1: {}", res1);
  println("2: {}", res2);

  return 0;
}
