#include <algorithm>
#include <bitset>
#include <cctype>
#include <charconv>
#include <deque>
#include <ios>
#include <iostream>
#include <limits>
#include <memory>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

using uint8 = std::uint8_t;
using int8 = std::int8_t;
using uint32 = std::uint32_t;
using int32 = std::int32_t;
using uint64 = std::uint64_t;
using int64 = std::int64_t;

using uint = uint32;
using string = std::string;
using namespace std::literals;
using size_t = std::size_t;

struct Point {
  int x, y;

  constexpr Point() {}
  constexpr Point(int x, int y) : x(x), y(y) {}
  constexpr Point(size_t x, size_t y) : x(x), y(y) {}

  friend Point operator+(const Point &a, const Point &b) { return {a.x + b.x, a.y + b.y}; }

  friend bool operator==(const Point &a, const Point &b) { return a.x == b.x && a.y == b.y; }
};

constexpr const uint MAX_H = std::numeric_limits<uint>::max();

struct Data {
  uint h = MAX_H;
  uint len = MAX_H;
  Point prev;

  Data() {}
  Data(char c) : h(c) {}
};

using Row = std::vector<Data>;
using Map = std::vector<Row>;
using Queue = std::deque<Point>;

const std::vector<Point> dirs = {{-1, 0}, {+1, 0}, {0, -1}, {0, +1}};

static Data &
get(Map &map, const Point &p) {
  static Data Invalid = Data{};

  if (0 <= p.x && p.x < map.size()) {
    auto &row = map[p.x];
    if (0 <= p.y && p.y < row.size()) {
      return row[p.y];
    }
  }
  return Invalid;
}

int
main() {
  Map map;
  Point goal;
  Queue queue;
  string line;
  while (std::cin >> line) {
    Row row;
    row.reserve(line.size());
    for (auto c : line) {
      switch (c) {
        case 'S':
          queue.emplace_back(map.size(), row.size());
          c = 'a';
          row.emplace_back(c);
          row.back().len = 0;
          continue;
        case 'E':
          goal = {map.size(), row.size()};
          c = 'z';
          break;
      }
      row.emplace_back(c);
    }
    map.emplace_back(std::move(row));
  }
  while (!queue.empty()) {
    Point p = queue.front();
    queue.pop_front();
    Data &data = map[p.x][p.y];
    uint len = data.len + 1;
    for (const auto &d : dirs) {
      auto pp = p + d;
      Data &next = get(map, pp);
      if (next.h <= data.h + 1 && next.len > len) {
        next.len = len;
        next.prev = p;
        if (pp == goal) {
          std::cout << len << "\n";
          return 0;
        }
        queue.push_back(pp);
      }
    }
  }
  return -1;
}
