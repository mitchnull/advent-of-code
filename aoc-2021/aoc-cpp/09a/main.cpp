#include <ios>
#include <iostream>
#include <cctype>
#include <limits>
#include <deque>
#include <string>
#include <vector>
#include <bitset>
#include <algorithm>
#include <ranges>
#include <charconv>

using uint8 = std::uint8_t;
using int8 = std::int8_t;
using uint32 = std::uint32_t;
using int32 = std::int32_t;
using uint64 = std::uint64_t;
using int64 = std::int64_t;

using uint = uint32;
using Item = uint8;

class Board {
public:
  const uint width, height;

  Board(uint width, uint height) :
    width(width),
    height(height),
    items(width * height, 0) {
  }

  Item &
  operator()(uint x, uint y) {
    return items[idx(x, y)];
  }

  Item
  operator()(uint x, uint y) const {
    return items[idx(x, y)];
  }

  auto
  begin() const {
    return items.begin();
  }

  auto
  end() const {
    return items.end();
  }

  friend std::ostream &operator<<(std::ostream &os, const Board &board) {
    for (uint y = 0; y < board.height; ++y) {
      for (uint x = 0; x < board.width; ++x) {
        std::cout << +board(x, y);
      }
      std::cout << "\n";
    }
    return os;
  }
private:
  std::vector<Item> items;

  uint
  idx(uint x, uint y) const {
    return y * width + x;
  }
};

struct Point {
  uint x, y;
};

constexpr const Item OFF_BOARD = std::numeric_limits<Item>::max();

static Item
get(const Board &board, uint x, uint y) {
  if (x < 0 || x >= board.width || y < 0 || y >= board.height) {
    return OFF_BOARD;
  }
  return board(x, y);
}

static uint
riskLevel(const Board &board, uint x, uint y) {
  uint level = board(x, y);
  if (get(board, x - 1, y) <= level
      || get(board, x + 1, y) <= level
      || get(board, x, y - 1) <= level
      || get(board, x, y + 1) <= level) {
    return 0;
  }
  return level + 1;
}


static uint
check(const Board& board) {
  uint res = 0;
  for (uint y = 0; y < board.height; ++y) {
    for (uint x = 0; x < board.width; ++x) {
      res += riskLevel(board, x, y);
    }
  }
  return res;
}

int
main() {
  std::string line;
  std::vector<std::string> lines;
  while (std::cin >> line) {
    lines.push_back(line);
  }
  Board board(line.size(), lines.size());
  for (uint y = 0; y < board.height; ++y) {
    for (uint x = 0; x < board.width; ++x) {
      board(x, y) = lines[y][x] - '0';
    }
  }
  // std::cout << board << "\n";
  std::cout << check(board) << "\n";
  return 0;
}
