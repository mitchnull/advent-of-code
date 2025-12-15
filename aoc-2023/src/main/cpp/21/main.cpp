#include "../utils.h"
#include <iostream>
#include <unordered_set>

/* ------------------------------------------------------------------------ */

using Board = Grid<>;
using Num = int64_t;
using Places = std::unordered_set<Pos>;

static std::pair<Num, Board>
solve1(Board board, Pos pos, Num steps) {
  Places curr{pos};
  for (int i = 0; i < steps; ++i) {
    Places next;
    for (auto p : curr) {
      for (auto dir : DIRS) {
        auto pp = p + dir;
        if (board[pp] != '#') {
          next.insert(pp);
        }
      }
    }
    std::swap(curr, next);
    // if (curr.size() == next.size()) {
    //   break;
    // }
  }
  for (auto p : curr) {
    board[p] = 'O';
  }
  return {curr.size(), board};
}

/* ------------------------------------------------------------------------ */

int
main() {
  std::vector<std::string> lines;
  std::string line;
  while (std::getline(std::cin, line)) {
    lines.push_back(line);
  }
  Board board = Board(lines, '#');
  Pos startPos = (board.iter() | views::filter([](auto i) { return i.v == 'S'; }) | views::transform([](auto i) {
    return Pos{i.x, i.y};
  })).front();

  auto [res1, board1] = solve1(board, startPos, 5);
  std::cout << "@@@ 5: " << res1 << std::endl << board1 << std::endl;
  std::cout << "---" << std::endl;
  auto [res2, board2] = solve1(board, startPos, 16);
  std::cout << "@@@ 16: " << res2 << std::endl << board2 << std::endl;
  auto [res3, board3] = solve1(board, startPos, 27);
  std::cout << "@@@ 27: " << res3 << std::endl << board3 << std::endl;

  auto [res1000, board1000] = solve1(board, startPos, 1000);
  std::cout << "@@@ 1000: " << res1000 << std::endl << board1000 << std::endl;
  auto [res1001, board1001] = solve1(board, startPos, 1001);
  std::cout << "@@@ 1001: " << res1001 << std::endl << board1001 << std::endl;

  return 0;

  std::cout << "---" << std::endl;
  auto [all, fullBoard] = solve1(board, startPos, board.w() + board.h());

  for (int i = 0; i < board.w() + board.h(); ++i) {
    auto [resi, bi] = solve1(board, {board.w() / 2, 0}, i);
    std::cout << "@@@ " << i << ": " << resi << std::endl;
    std::cout << bi << std::endl;
  }

  // const Num STEPS2 = 26501365L;

  std::cout << res1 << std::endl;
  std::cout << all << std::endl;
}
