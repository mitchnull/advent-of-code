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
  }
  for (auto p : curr) {
    board[p] = 'O';
  }
  return {curr.size(), board};
}

static Num
solve2(const Board& board, Pos pos, Num steps) {
  // Notes:
  // - board size is 131x131
  // - we can reach the extents as the obstacles do not create too much disturbance anywhere
  // - there's also a nice "clear path" where the iteration stops
  // - we can reach all sides in a straight line
  // - step count is 26501365 = 131 * 202300 + 65, so we reach to just the edges of the tiles
  // - there are a few kinds of "small boards" stitched together to create the large board
  // - we sum up the values for these small boards in appropriate quantities (see samples)
  int bs = board.h();
  int half = bs - pos.y - 1;
  int rep = (steps - half) / bs;

  Num odds1 = rep - 1;
  Num odds = (odds1 - 1) * (rep - 1);
  Num evens1 = rep;
  Num evens = ((evens1 - 1) * rep);

  auto [odd, oddBoard] = solve1(board, pos, bs + half - 1);
  auto [even, evenBoard] = solve1(board, pos, bs + half);

  auto [slb, slbBoard] = solve1(board, Pos{bs - 1, bs - 1}, half - 1);
  auto [srb, srbBoard] = solve1(board, Pos{0, bs - 1}, half - 1);
  auto [llb, llbBoard] = solve1(board, Pos{bs - 1, bs - 1}, bs + half - 1);
  auto [lrb, lrbBoard] = solve1(board, Pos{0, bs - 1}, bs + half - 1);

  auto [slt, sltBoard] = solve1(board, Pos{0, 0}, half - 1);
  auto [srt, srtBoard] = solve1(board, Pos{bs - 1, 0}, half - 1);
  auto [llt, lltBoard] = solve1(board, Pos{0, 0}, bs + half - 1);
  auto [lrt, lrtBoard] = solve1(board, Pos{bs - 1, 0}, bs + half - 1);

  auto [left, lBoard] = solve1(board, Pos{bs - 1, bs / 2}, bs - 1);
  auto [right, rBoard] = solve1(board, Pos{0, bs / 2}, bs - 1);
  auto [top, tBoard] = solve1(board, Pos{bs / 2,bs - 1}, bs - 1);
  auto [bottom, bBoard] = solve1(board, Pos{bs / 2, 0}, bs - 1);

  return (odds + odds1) * odd + (evens + evens1) * even + (slb + srb + slt + srt) * rep + (llb + lrb + llt + lrt) * (rep - 1) + top + bottom + left + right;
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
  auto res2 = solve2(board, startPos, 26501365L);

  println("1: {}", res1);
  println("2: {}", res2);
}
