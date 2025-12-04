#include "../utils.h"

using Board = Grid<>;

static int
rolls(const Board& board, int x, int y) {
  int res{};
  for (int yy = y - 1; yy <= y + 1; ++yy) {
    for (int xx = x - 1; xx <= x + 1; ++xx) {
      res += board[xx, yy] == '@';
    }
  }
  return res;
}

static std::pair<int, Board>
solve1(const Board& board) {
  int res{};
  Board nb{board};
  for (int y = 0; y < board.h(); ++y) {
    for (int x = 0; x < board.w(); ++x) {
      if (board[x, y] == '@' && rolls(board, x, y) <= 4) {
        ++res;
        nb[x, y] = '.';
      }
    }
  }
  return {res, nb};
}

static int
solve2(Board board) {
  int res{}, r;
  for (std::tie(r, board) = solve1(board); r > 0; std::tie(r, board) = solve1(board)) {
    res += r;
  }
  return res;
}

/* ------------------------------------------------------------------------ */

int
main() {
  int res1{}, res2{};
  Board board = Board::read(std::cin, '.');

  std::tie(res1, board) = solve1(board);
  res2 = res1 + solve2(board);

  println("1: {}", res1);
  println("2: {}", res2);

  return 0;
}
