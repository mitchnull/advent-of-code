#include <iostream>
#include <string>
#include <algorithm>
#include <numeric>
#include "../utils.h"

/* ------------------------------------------------------------------------ */

using Num = std::int64_t;
struct Tile {
  char c;
  Num area;
};

using Board= Grid<Tile>;
using string = std::string;
using Line = std::pair<Pos, Pos>;
using Lines = std::vector<Line>;

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

static void
fill(Board& board, Pos pos) {
  if (board[pos].c != '.') {
    return;
  }
  board[pos].c = ' ';
  for (auto dir : DIRS) {
    fill(board, pos + dir);
  }
}

static char
tileChar(const Lines& lines, int x, int y) {
  return (std::find_if(lines.begin(), lines.end(), [=](auto& l) { return l.first.x <= x && x <= l.second.x && l.first.y <= y && y <= l.second.y; }) != lines.end())
    ? '#'
    : '.';
}

static Board
convertToBoard(const Entries& entries) {
  Lines lines;
  std::vector<Num> xs, ys;
  Pos p{};
  xs.push_back(p.x);
  ys.push_back(p.y);
  for (auto& e : entries) {
    Pos pp = p + (e.dir * e.steps);
    xs.push_back(pp.x);
    ys.push_back(pp.y);
    lines.push_back({{std::min(p.x, pp.x), std::min(p.y, pp.y)}, {std::max(p.x, pp.x), std::max(p.y, pp.y)}});
    p = pp;
  }
  std::sort(xs.begin(), xs.end());
  xs.erase(std::unique(xs.begin(), xs.end()), xs.end());
  std::sort(ys.begin(), ys.end());
  ys.erase(std::unique(ys.begin(), ys.end()), ys.end());
  xs.push_back(xs.back() + 2);
  ys.push_back(ys.back() + 2);
  
  Board board(xs.size() * 2, ys.size() * 2);
  Num py = ys.front() - 2;
  for (int yi = 0; yi < ys.size(); ++yi) {
    auto y = ys[yi];
    Num px = xs.front() - 2;
    for (int xi = 0; xi < xs.size(); ++xi) {
      auto x = xs[xi];
      board[xi * 2,     yi * 2] = {tileChar(lines, px + 1, py + 1), (x - (px + 1)) * (y - (py + 1))};
      board[xi * 2 + 1, yi * 2] = {tileChar(lines, x, py + 1), 1 * (y - (py + 1))};
      board[xi * 2,     yi * 2 + 1] = {tileChar(lines, px + 1, y), (x - (px + 1)) * 1};
      board[xi * 2 + 1, yi * 2 + 1] = {tileChar(lines, x, y), 1 * 1};
      px = x;
    }
    py = y;
  }
  return board;
}

static Num
solve(const Entries& entries) {
  auto board = convertToBoard(entries);
  fill(board, {0, 0});
  return std::reduce(board.begin(), board.end(), Num{}, [](Num a, auto t) { return t.c != ' ' ? a + t.area : a; });
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
  auto res2 = solve(entries2);

  std::cout << res1 << "\n";
  std::cout << res2 << "\n";
}
