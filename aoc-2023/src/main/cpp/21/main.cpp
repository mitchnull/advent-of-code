#include "../utils.h"
#include <iostream>
#include <unordered_set>

/* ------------------------------------------------------------------------ */

using Board = Grid<>;
using Num = int64_t;
using Places = std::unordered_set<Pos>;

static std::pair<Num, Board>
solve1(Board board, Pos pos, Num steps, bool wrap = false) {
  Places curr{pos};
  for (int i = 0; i < steps; ++i) {
    Places next;
    for (auto p : curr) {
      for (auto dir : DIRS) {
        auto pp = p + dir;
        if (wrap) {
          pp = { (pp.x + board.w()) % board.w(), (pp.y + board.h()) % board.h() };
        }
        if (board[pp] != '#') {
          next.insert(pp);
        }
      }
    }
    std::swap(curr, next);
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

  auto [res1, board1] = solve1(board, startPos, 64);

  // Notes:
  // - board size is 131x131
  // - board gets filled as the obstacles do not create closed areas
  // - we can reach all sides in a straight line
  // - step count is 26501365 = 131 * 202300 + 65, so we reach to just the edges of the tiles with 1 step is on a new tiles
  for (int steps : { 10, 11, 31, 32 }) {
    const int TS = 7;
    const int EXT = (steps - (TS / 2)) / TS + 1;
    const int N = (EXT * 2 + 1) * TS;
    Board tb = Board(N, N, '.', '#');
    auto [tr, tbr] = solve1(tb, Pos{N / 2, N / 2}, steps);
    println("test board {}x{} steps {}: \n{}", TS, TS, steps, tbr);
  }

  // const Num STEPS2 = 26501365L;

  std::cout << res1 << std::endl;
}
