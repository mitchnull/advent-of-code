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
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_set>
#include <variant>
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
  uint x, y;
};

struct Line {
  Point a, b;
};

constexpr const uint P = 500;
constexpr const uint W = P + P;

using Row = std::bitset<W>;
static const Row ALL_SET = Row().flip();

using Map = std::vector<Row>;

static void
ensureRow(Map& map, uint row) {
  if (map.size() <= row) {
    map.resize(row + 1, ALL_SET);
  }
}

static void
addLine(Map& map, const Line& line) {
  ensureRow(map, std::max(line.a.y, line.b.y));
  if (line.a.y == line.b.y) {
    uint sx = std::min(line.a.x, line.b.x);
    uint ex = std::max(line.a.x, line.b.x);
    while (sx <= ex) {
      map[line.a.y][sx++] = false;
    }
  } else {
    uint sy = std::min(line.a.y, line.b.y);
    uint ey = std::max(line.a.y, line.b.y);
    while (sy <= ey) {
      map[sy++][line.a.x] = false;
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

int
main() {
  Map map;
  string line;
  while (std::getline(std::cin, line)) {
    parse(line, map);
  }
  map.push_back(ALL_SET);
  uint res = 0;
  Row p;
  p[P] = true;
  ++res;
  for (uint y = 1; y < map.size(); ++y) {
    p = map[y] & (p | (p << 1) | (p >> 1));
    res += p.count();
  }
  std::cout << res << "\n";
}
