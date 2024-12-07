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

using uint8 = std::uint8_t;
using int8 = std::int8_t;
using uint32 = std::uint32_t;
using int32 = std::int32_t;
using uint64 = std::uint64_t;
using int64 = std::int64_t;

using uint = uint32;
using string = std::string;

struct Coords {
  int x, y;
};

static bool
operator==(const Coords& a, const Coords& b) {
  return a.x == b.x && a.y == b.y;
}

namespace std {
  template<>
  struct hash<Coords> {
    std::size_t operator()(const Coords &c) const {
      return std::hash<int>{}(c.x) ^ (std::hash<int>{}(c.y) << 1);
    }
  };
}

static int
d(int a, int b) {
  return std::abs(a - b);
}

static int
sgn(int x) {
  return (x > 0) - (x < 0);
}

static void
move(Coords& head, char dir) {
  switch (dir) {
    case 'L': --head.x; return;
    case 'R': ++head.x; return;
    case 'U': ++head.y; return;
    case 'D': --head.y; return;
  }
}

static void
follow(const Coords& head, Coords& tail) {
  if (d(head.x, tail.x) <= 1 && d(head.y, tail.y) <= 1) {
    return;
  }
  tail.x += sgn(head.x - tail.x);
  tail.y += sgn(head.y - tail.y);
};

using CoordsSet = std::unordered_set<Coords>;

int
main() {
  char dir;
  int count;
  Coords head{}, tail{};

  CoordsSet visited;
  while (std::cin >> dir >> count) {
    while (count--) {
      move(head, dir);
      follow(head, tail);
      visited.insert(tail);
    }
  }
  std::cout << visited.size() << "\n";
  return 0;
}
