#include <iostream>
#include <unordered_set>
#include "../utils.h"

/* ------------------------------------------------------------------------ */

using Board = Grid<>;
using Num = int;
using Places = std::unordered_set<Pos>;

static Num
solve(const Board& board, Pos pos, Num steps) {
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
  }
  return curr.size();
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
  Pos startPos = (board.iter() | views::filter([](auto i) { return i.v == 'S'; }) | views::transform([](auto i) { return Pos{i.x, i.y}; })).front();

  Num res1 = solve(board, startPos, 64);
  std::cout << res1 << "\n";
}
