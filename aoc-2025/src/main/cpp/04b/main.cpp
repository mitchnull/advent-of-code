#include <iostream>
#include <cstdint>
#include <cmath>
#include <unordered_set>
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

static int
solve1(Board& board) {
  int res{};
  Board orig{board};
  for (int y = 0; y < orig.h(); ++y) {
    for (int x = 0; x < orig.w(); ++x) {
      if (orig[x, y] == '@' && rolls(orig, x, y) <= 4) {
        ++res;
        board[x, y] = '.';
      }
    }
  }
  return res;
}

/* ------------------------------------------------------------------------ */

int
main() {
  std::vector<std::string> lines;
  std::string line;
  int res1{}, res2{}, r{};
  while (std::getline(std::cin, line)) {
    lines.push_back(line);
  }
  Board board = Board(lines, '.');

  res1 = res2 = solve1(board);
  while ((r = solve1(board)) > 0) {
    res2 += r;
  }

  println("1: {}", res1);
  println("2: {}", res2);

  return 0;
}
