#include "../utils.h"
#include <iostream>

/* ------------------------------------------------------------------------ */

using Board = Grid<>;
using Num = int;
using Path = std::vector<Pos>;

/* ------------------------------------------------------------------------ */

static Path
findPath(const Board& board, Pos p, Pos end) {
  auto path = Path{p};
  auto prev = p;
  while (p != end) {
    for (Dir d : DIRS) {
      auto pp = p + d;
      if (pp != prev && board[pp] != '#') {
        prev = p;
        p = pp;
        path.push_back(p);
        break;
      }
    }
  }
  return path;
}

static std::pair<Num, Num>
solve(const Path& path, int maxCheat1, int maxCheat2, int minGain) {
  Num res1 = 0, res2 = 0;
  for (auto i = path.begin(), end = path.end(), endi = end - minGain; i < endi; ++i) {
    for (auto j = i + minGain; j < end; ++j) {
      auto d = std::abs(j->x - i->x) + std::abs(j->y - i->y);
      if (((j - i) - d) >= minGain) {
        res1 += d <= maxCheat1;
        res2 += d <= maxCheat2;
      }
    }
  }
  return {res1, res2};
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
  Pos endPos = (board.iter() | views::filter([](auto i) { return i.v == 'E'; }) | views::transform([](auto i) { return Pos{i.x, i.y}; })).front();

  auto path = findPath(board, startPos, endPos);
  auto [res1, res2] = solve(path, 2, 20, 100);

  std::cout << "1: " << res1 << "\n";
  std::cout << "2: " << res2 << "\n";

  return 0;
}
