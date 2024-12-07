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
#include <numeric>
#include <memory>
#include <unordered_set>
#include <string_view>
#include <variant>
#include <sstream>

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
  uint x, y;
};

struct Line {
  Point a, b;
};

constexpr const uint P = 500;
constexpr const uint W = P + P;

using Map = std::vector<std::vector<bool>>;

static void
ensureRow(Map& map, uint row) {
  if (map.size() <= row) {
    map.resize(row + 1, std::vector<bool>(W));
  }
}

static void
addLine(Map& map, const Line& line) {
  ensureRow(map, std::max(line.a.y, line.b.y));
  if (line.a.y == line.b.y) {
    uint sx = std::min(line.a.x, line.b.x);
    uint ex = std::max(line.a.x, line.b.x);
    while (sx <= ex) {
      map[line.a.y][sx++] = true;
    }
  } else {
    uint sy = std::min(line.a.y, line.b.y);
    uint ey = std::max(line.a.y, line.b.y);
    while (sy <= ey) {
      map[sy++][line.a.x] = true;
    }
  }
}


static void
parse(const string& line, Map& map) {
  uint x, y, lx, ly;
  string str;
  char c;
  std::stringstream ss{line};
  ss >> lx >> c >> ly;
  while (ss >> str >> x >> c >> y) {
    addLine(map, {{lx, ly}, {x, y}});
    lx = x;
    ly = y;
  }
}

static bool
fall(Map& map, Point& p) {
  uint y1 = p.y + 1;
  if (!map[y1][p.x]) {
    p.y = y1;
    return true;
  } else if (!map[y1][p.x - 1]) {
    p.x = p.x - 1;
    p.y = y1;
    return true;
  } else if (!map[y1][p.x + 1]) {
    p.x = p.x + 1;
    p.y = y1;
    return true;
  }
  map[p.y][p.x] = true;
  return false;
}

int
main() {
  Map map;
  string line;
  while (std::getline(std::cin, line)) {
    parse(line, map);
  }
  const uint d = map.size();
  map.push_back(std::vector<bool>(W, false));
  uint res = 0;
  while (true) {
    Point p = {P, 0};
    while (p.y < d && fall(map, p)) {
      continue;
    }
    if (p.y >= d) {
      break;
    }
    ++res;
  }
  std::cout << res << "\n";
}
