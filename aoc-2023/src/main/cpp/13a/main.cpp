#include <ios>
#include <iostream>
#include <cctype>
#include <cstdint>
#include <limits>
#include <deque>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <bitset>
#include <algorithm>
#include <ranges>
#include <sstream>
#include <numeric>

using Num = std::uint64_t;

namespace ranges = std::ranges;
namespace views = ranges::views;

using Board = std::vector<std::string>;

struct Dir {
  int di, dj;

  Dir swap() const {
    return {dj, di};
  }
};

struct Pos {
  int i, j;

  Pos& operator+=(Dir dir) {
    i += dir.di;
    j += dir.dj;
    return *this;
  }
  Pos& operator-=(Dir dir) {
    i -= dir.di;
    j -= dir.dj;
    return *this;
  }
  Pos proj(Dir dir) const {
    return {i * dir.di, j * dir.dj};
  }

  friend auto operator<=>(const Pos&, const Pos&) = default;

  bool allLess(const Pos& other) const {
    return i < other.i && j < other.j;
  }
};

static void
swap(Pos& a, Pos& b) {
  std::swap(a.i, b.i);
  std::swap(a.j, b.j);
}

static Pos
endPos(const Board& board) {
  return Pos(board.size(), board.front().size());
}

static bool
isSame(const Board& board, Pos p, Pos q, Dir dir) {
  const Pos end = endPos(board);
  while (p.allLess(end)) {
    if (board[p.i][p.j] != board[q.i][q.j]) {
      return false;
    }
    p += dir;
    q += dir;
  }
  return true;
}

static bool
check(const Board& board, Pos first, Pos last, Dir dir) {
  if (last < first) {
    swap(last, first);
  }
  Dir xdir = dir.swap();
  while (first < last) {
    if (!isSame(board, first, last, xdir)) {
      return false;
    }
    first += dir;
    last -= dir;
  }
  return first != last;
}

static Pos
findSame(const Board& board, Pos edge, Dir dir, Pos pos) {
  const Pos end = endPos(board).proj(dir);
  Dir xdir = dir.swap();
  while (pos < end && !isSame(board, edge, pos, xdir)) {
    pos += dir;
  }
  return pos;
}

static Num
calc(const Board& board, Dir dir) {
  const Pos end = endPos(board);
  for (Pos edge : {Pos{0, 0}, end.proj(dir) -= dir}) {
    for (auto pos = findSame(board, edge, dir, {0, 0}); pos.allLess(end); pos = findSame(board, edge, dir, pos += dir)) {
      if (pos != edge && check(board, edge, pos, dir)) {
        return (dir.di * (pos.i + edge.i) + dir.dj * (pos.j + edge.j)) / 2  + 1;
      }
    }
  }
  return 0;
}

static Num
calc(const Board& board) {
  if (board.empty()) {
    return 0;
  }
  return calc(board, {0,1}) + 100 * calc(board, {1,0});
}

int
main() {
  Board board;
  std::string line;
  Num sum = 0;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      sum += calc(board);
      board.clear();
      continue;
    }
    board.push_back(line);
  }
  sum += calc(board);
  std::cout << sum << "\n";
  return 0;
}
