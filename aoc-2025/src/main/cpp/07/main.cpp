#include "../utils.h"
#include <algorithm>

using Num = int64_t;
using Board = Grid<>;

static std::pair<Num, Num>
solve(const Board &board) {
  Num splits{};
  auto ways = ranges::subrange(board.begin(), board.begin() + board.w()) |
      views::transform([](auto c) { return c == 'S'; }) | ranges::to<std::vector<Num>>();
  for (int y = 1; y < board.h(); ++y) {
    for (int x = 0; x < board.w(); ++x) {
      if (board[x, y] == '^') {
        splits += ways[x] != 0;
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
  return {splits, ranges::fold_left(ways, Num{}, std::plus<>{})};
}

/* ------------------------------------------------------------------------ */

int
main() {
  Board board = Board::read(std::cin, '.');

  auto res = solve(board);
  println("1: {}", res.first);
  println("2: {}", res.second);

  return 0;
}
