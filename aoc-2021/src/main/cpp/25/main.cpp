#include "../utils.h"

using Num = int64_t;
using Board = Grid<>;

static int
move(Board &b, char cuc, Dir d) {
  int moves = 0;
  for (int y = 0; y < b.h(); ++y) {
    for (int x = 0; x < b.w(); ++x) {
      if (b[x, y] == cuc) {
        int nx = (x + d.dx) % b.w();
        int ny = (y + d.dy) % b.h();
        if (b[nx, ny] == '.') {
          b[x, y] = '@';
          ++moves;
        }
      }
    }
  }
  if (!moves) {
    return false;
  }
  for (int y = 0; y < b.h(); ++y) {
    for (int x = 0; x < b.w(); ++x) {
      if (b[x, y] == '@') {
        int nx = (x + d.dx) % b.w();
        int ny = (y + d.dy) % b.h();
        b[nx, ny] = cuc;
        b[x, y] = '.';
      }
    }
  }
  return moves;
}

static Num
solve1(Board b) {
  for (Num moves = 1;; ++moves) {
    if (move(b, '>', Dir{1, 0}) + move(b, 'v', Dir{0, 1}) == 0) {
      return moves;
    }
  }
}

/* ------------------------------------------------------------------------ */

int
main() {
  Board b = Board::read();

  println("1: {}", solve1(b));

  return 0;
}
