#include "../utils.h"
#include <algorithm>

using Board = Grid<>;

static int
rolls(const Board &board, int x, int y) {
  int res{};
  for (int yy = y - 1; yy <= y + 1; ++yy) {
    for (int xx = x - 1; xx <= x + 1; ++xx) {
      res += board[xx, yy] != '.';
    }
  }
  return res;
}

static int
solve1(Board &board) {
  int res{};
  for (int y = 0; y < board.h(); ++y) {
    for (int x = 0; x < board.w(); ++x) {
      if (board[x, y] == '@' && rolls(board, x, y) <= 4) {
        ++res;
        board[x, y] = '-';
      }
    }
  }
  std::for_each(board.begin(), board.end(), [](auto &c) { c = (c == '-' ? '.' : c); });
  return res;
}

/* ------------------------------------------------------------------------ */

int
main() {
  int res1{}, res2{}, r;
  Board board = Board::read(std::cin, '.');

  res1 = res2 = solve1(board);
  while ((r = solve1(board)) > 0) {
    res2 += r;
  }

  println("1: {}", res1);
  println("2: {}", res2);

  return 0;
}
