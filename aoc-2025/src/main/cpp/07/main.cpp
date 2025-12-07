#include "../utils.h"
#include <algorithm>

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
  return res;
}

static Num
solve2(Board board) {
  auto ways = std::vector<Num>(board.w());
  ++ways[std::find(board.begin(), board.end(), 'S') - board.begin()];
  for (int y = 1; y < board.h(); ++y) {
    for (int x = 0; x < board.w(); ++x) {
      if (board[x, y] == '^') {
        if (x > 0) {
          ways[x - 1] += ways[x];
        }
        if (x < ways.size() - 1) {
          ways[x + 1] += ways[x];
        }
        ways[x] = 0;
      }
    }
  }
  return ranges::fold_left(ways, Num{}, std::plus<>{});
}

/* ------------------------------------------------------------------------ */

int
main() {
  Board board = Board::read(std::cin, '.');

  println("1: {}", solve1(board));
  println("2: {}", solve2(board));

  return 0;
}
