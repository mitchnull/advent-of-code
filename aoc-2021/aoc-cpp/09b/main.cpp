#include <algorithm>
#include <bitset>
#include <cctype>
#include <charconv>
#include <deque>
#include <ios>
#include <iostream>
#include <limits>
#include <numeric>
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
using Item = uint8;

constexpr const Item OFF_BOARD = std::numeric_limits<Item>::max();
constexpr const Item BASIN = OFF_BOARD - 1;
constexpr const Item PLATEAU = 9;

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
        if (board(x, y) == BASIN) {
          std::cout << '*';
        } else {
          std::cout << +board(x, y);
        }
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

static Item
get(const Board &board, uint x, uint y) {
  if (x < 0 || x >= board.width || y < 0 || y >= board.height) {
    return OFF_BOARD;
  }
  return board(x, y);
}

static uint
checkBasin(Board &board, uint x, uint y, std::vector<Point> &queue) {
  if (get(board, x, y) < PLATEAU) {
    board(x, y) = BASIN;
    queue.emplace_back(x, y);
    return 1;
  }
  return 0;
}

static uint
basinSize(Board &board, uint x, uint y) {
  std::vector<Point> queue;
  board(x, y) = BASIN;
  queue.emplace_back(x, y);
  uint res = 1;
  while (!queue.empty()) {
    auto [x, y] = queue.back();
    queue.pop_back();
    res += checkBasin(board, x - 1, y, queue);
    res += checkBasin(board, x + 1, y, queue);
    res += checkBasin(board, x, y - 1, queue);
    res += checkBasin(board, x, y + 1, queue);
  }
  // std::cout << board << res << "\n";
  return res;
}

static uint
check(Board& board) {
  std::vector<uint> basinSizes;
  for (uint y = 0; y < board.height; ++y) {
    for (uint x = 0; x < board.width; ++x) {
      if (board(x, y) < PLATEAU) {
        basinSizes.push_back(basinSize(board, x, y));
      }
    }
  }
  std::sort(basinSizes.begin(), basinSizes.end(), std::greater<>());
  return std::accumulate(basinSizes.begin(), basinSizes.begin() + 3, 1, [](auto a, auto b) {
    return a * b;
  });
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
