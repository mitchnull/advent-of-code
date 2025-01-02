#include <iostream>
#include <string>
#include <algorithm>
#include "../utils.h"

/* ------------------------------------------------------------------------ */

using Board = Grid<>;
using Num = int;
using string = std::string;

struct Entry {
  Dir dir;
  int steps;
};

using Entries = std::vector<Entry>;

static Dir
toDir(char c) {
  switch (c) {
    case 'U': case '3': return {0, -1};
    case 'D': case '1': return {0, 1};
    case 'L': case '2': return {-1, 0};
    case 'R': case '0': return {1, 0};
  }
  return {};
}

static std::tuple<Pos, int, int>
findExtents(const Entries& entries) {
  Pos p{}, tl{}, br{};
  for (auto& e : entries) {
    p += (e.dir * e.steps);
    tl.x = std::min(tl.x, p.x);
    tl.y = std::min(tl.y, p.y);
    br.x = std::max(br.x, p.x);
    br.y = std::max(br.y, p.y);
  }
  return {{p.x - tl.x + 1, p.y - tl.y + 1}, br.x - tl.x + 3, br.y - tl.y + 3};
}

static void
fill(Board& board, Pos pos) {
  if (board[pos] != '.') {
    return;
  }
  board[pos] = ' ';
  for (auto dir : DIRS) {
    fill(board, pos + dir);
  }
}

static Num
solve(const Entries& entries) {
  auto [pos, w, h] = findExtents(entries);
  Board board(w, h, '.');
  board[pos] = '#';
  for (auto& e : entries) {
    for (int i = 0; i < e.steps; ++i) {
      pos += e.dir;
      board[pos] = '#';
    }
  }
  fill(board, {0, 0});
  return std::count_if(board.begin(), board.end(), [](auto c) { return c != ' '; });
}


/* ------------------------------------------------------------------------ */

int
main() {
  char dir;
  int steps;
  string color;

  Entries entries1;
  Entries entries2;
  while (std::cin >> dir >> steps >> color) {
    entries1.emplace_back(toDir(dir), steps);
    entries2.emplace_back(toDir(color[7]), std::stoi(color.substr(2, 5), nullptr, 16));
  }
  auto res1 = solve(entries1);
  // auto res2 = solve(entries2);

  std::cout << res1 << "\n";
  // std::cout << res2 << "\n";
}
