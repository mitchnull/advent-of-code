#include "../utils.h"

using Num = int64_t;
using Board = Grid<>;

static Num
solve1(Board board) {
  Num res{};
  for (int y = 1; y < board.h(); ++y) {
    for (int x = 0; x < board.w(); ++x) {
      if (board[x, y - 1] == '|' || board[x, y - 1] == 'S') {
        if (board[x, y] == '^') {
          ++res;
          board[x - 1, y] = board[x + 1, y] = '|';
        } else {
          board[x, y] = '|';
        }
      }
    }
  }
  // println("{}", board);
  return res;
}

/* ------------------------------------------------------------------------ */

int
main() {
  Num res1{}, res2{};

  Board board = Board::read(std::cin, '.');

  println("1: {}", solve1(board));
  println("2: {}", res2);

  return 0;
}
