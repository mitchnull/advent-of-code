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
  int res = 0;
  while (true) {
    auto [c, nb] = solve1(board);
    if (c == 0) {
      return res;
    }
    res += c;
    board = nb;
  }
}

/* ------------------------------------------------------------------------ */

int
main() {
  std::vector<std::string> lines;
  std::string line;
  int res1{}, res2{};
  while (std::getline(std::cin, line)) {
    lines.push_back(line);
  }
  Board board = Board(lines, '.');

  res1 = solve1(board).first;
  res2 = solve2(board);

  println("1: {}", res1);
  println("2: {}", res2);

  return 0;
}
