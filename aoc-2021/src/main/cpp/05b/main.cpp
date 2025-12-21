#include <algorithm>
#include <bitset>
#include <cctype>
#include <charconv>
#include <deque>
#include <ios>
#include <iostream>
#include <limits>
#include <ranges>
#include <string>
#include <vector>

using uint8 = std::uint8_t;
using int8 = std::int8_t;
using uint32 = std::uint32_t;
using int32 = std::int32_t;
using uint64 = std::uint64_t;
using int64 = std::int64_t;

using uint = uint32;

class Board {
  uint width, height;
  std::vector<uint> items;

  uint idx(uint x, uint y) const { return y * width + x; }
public:
  Board(uint width, uint height) : width(width), height(height), items(width * height, 0) {}

  uint &operator()(uint x, uint y) { return items[idx(x, y)]; }

  uint operator()(uint x, uint y) const { return items[idx(x, y)]; }

  auto begin() const { return items.begin(); }

  auto end() const { return items.end(); }

  friend std::ostream &operator<<(std::ostream &os, const Board &board) {
    for (uint y = 0; y < board.height; ++y) {
      for (uint x = 0; x < board.width; ++x) {
        std::cout << board(x, y);
      }
      std::cout << "\n";
    }
    return os;
  }
};

struct Point {
  uint x, y;
};

struct Line {
  Point a, b;
};

using Lines = std::vector<Line>;

static uint
check(const Board &board) {
  return std::ranges::count_if(board, [](auto i) { return i > 1; });
}

static int
step(uint a, uint b) {
  if (a == b) {
    return 0;
  }
  return a < b ? 1 : -1;
}

static uint
dist(uint a, uint b) {
  return a < b ? b - a : a - b;
}

static void
draw(Board &board, const Line &line) {
  int hStep = step(line.a.x, line.b.x);
  int vStep = step(line.a.y, line.b.y);
  uint i = line.a.x;
  uint j = line.a.y;
  while (i != line.b.x || j != line.b.y) {
    ++board(i, j);
    i += hStep;
    j += vStep;
  }
  ++board(i, j);
}

static bool
isValid(const Line &line) {
  return line.a.x == line.b.x || line.a.y == line.b.y || dist(line.a.x, line.b.x) == dist(line.a.y, line.b.y);
}

int
main() {
  uint ax, ay, bx, by;
  char c;
  std::string str;
  Lines lines;
  while (std::cin >> ax >> c >> ay >> str >> bx >> c >> by) {
    Line line{Point{ax, ay}, Point{bx, by}};
    if (isValid(line)) {
      lines.emplace_back(Point{ax, ay}, Point{bx, by});
    }
  }
  uint width = 0;
  uint height = 0;
  for (const auto &line : lines) {
    width = std::max({width, line.a.x, line.b.x});
    height = std::max({height, line.a.y, line.b.y});
  }
  Board board(width + 1, height + 1);
  for (const auto &line : lines) {
    draw(board, line);
  }
  // std::cout << board << "\n";
  std::cout << check(board) << "\n";
  return 0;
}
