#include <iostream>
#include <numeric>
#include <string>
#include "../utils.h"

using Board = Grid<>;

/* ------------------------------------------------------------------------ */

static Dir
dir(char c) {
  switch (c) {
    case '^': return {0, -1};
    case '>': return {1, 0};
    case 'v': return {0, 1};
    case '<': return {-1, 0};
  }
  return {};
}

static Pos
move(Board& board, Pos p, Dir d) {
  auto pp = p + d;
  switch (board[pp]) {
    case '#':
      return p;
    case 'O':
      if (move(board, pp, d) == pp) {
        return p;
      }
  }
  std::swap(board[pp], board[p]);
  return pp;
}

static int
solve1(const std::vector<std::string>& lines, std::vector<char>& moves) {
  Board board = Board(lines);
  Pos p = (board.iter() | views::filter([](auto i) { return i.v == '@'; }) | views::transform([](auto i) { return Pos{i.x, i.y}; })).front();
  for (char c : moves) {
    p = move(board, p, dir(c));
  }
  auto gpsScores = board.iter() | views::filter([](auto i) { return i.v == 'O'; }) | views::transform([](auto i) { return i.y * 100 + i.x; });
  return std::reduce(gpsScores.begin(), gpsScores.end(), 0);
}

/* ------------------------------------------------------------------------ */

static char
left(char c) {
  switch (c) {
    case 'O': return '[';
    case '@': return '@';
  }
  return c;
}

static char
right(char c) {
  switch (c) {
    case 'O': return ']';
    case '#': return '#';
    case '@': return '.';
  }
  return c;
}

static Pos
otherSide(char c, Pos p) {
  switch (c) {
    case '[': return {p.x + 1, p.y};
    case ']': return {p.x - 1, p.y};
  }
  return p;
}

static Pos
move2(Board& board, Pos p, Dir d) {
  auto po = d.dy == 0 ? p : otherSide(board[p], p);
  auto pn = p + d;
  auto pon = po + d;

  if (board[pn] == '#' || board[pon] == '#') {
    return p;
  }
  if (board[pn] == '.' && board[pon] == '.') {
    if (po != p) {
      std::swap(board[po], board[pon]);
    }
    std::swap(board[p], board[pn]);
    return pn;
  }
  Board save = board;
  if ((board[pn] == '.' || move2(board, pn, d) != pn) && (board[pon] == '.' || move2(board, pon, d) != pon)) {
    return move2(board, p, d);
  }
  board = save;
  return p;
}

static int
solve2(const std::vector<std::string>& lines, std::vector<char>& moves) {
  Board board = Board(lines.front().size() * 2, lines.size());
  for (int y = 0; y < lines.size(); ++y) {
    const auto& line = lines[y];
    for (int x = 0; x < line.size(); ++x) {
      board[2 * x, y] = left(line[x]);
      board[2 * x + 1, y] = right(line[x]);
    }
  }

  Pos p = (board.iter() | views::filter([](auto i) { return i.v == '@'; }) | views::transform([](auto i) { return Pos{i.x, i.y}; })).front();
  for (char c : moves) {
    p = move2(board, p, dir(c));
  }
  auto gpsScores = board.iter() | views::filter([](auto i) { return i.v == '['; }) | views::transform([](auto i) { return i.y * 100 + i.x; });
  return std::reduce(gpsScores.begin(), gpsScores.end(), 0);
}

/* ------------------------------------------------------------------------ */

int
main() {
  std::vector<std::string> lines;
  std::string line;
  while (std::getline(std::cin, line) && !line.empty()) {
    lines.push_back(line);
  }
  auto moves = std::vector<char>(std::istream_iterator<char>(std::cin), std::istream_iterator<char>());

  int res1 = solve1(lines, moves);
  int res2 = solve2(lines, moves);

  std::cout << "1: " << res1 << "\n";
  std::cout << "2: " << res2 << "\n";

  return 0;
}
