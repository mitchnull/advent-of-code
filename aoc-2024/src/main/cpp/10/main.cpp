#include "../utils.h"
#include <cstdint>
#include <iostream>
#include <set>
#include <string>

using uint = std::uint32_t;

struct Place {
  int h;
  std::set<Pos> origins;
  int ways;
};

using PlaceMap = Grid<Place>;
using Num = int;

// ------------------------------------------------------------------------ //

static std::pair<Num, Num>
solve(PlaceMap& map) {
  for (auto [x, y, p] : map.iter()) {
    if (p.h == 0) {
      p.origins.emplace(x, y);
      p.ways = 1;
    }
  }
  for (int h = 0; h < 9; ++h) {
    for (auto [x, y, p] : map.iter()) {
      if (p.h == h) {
        for (auto d: DIRS) {
          auto xx = x + d.dx;
          auto yy = y + d.dy;
          auto& pp = map[xx, yy];
          if (pp.h == h + 1) {
            pp.origins.insert(p.origins.begin(), p.origins.end());
            pp.ways += p.ways;
          }
        }
      }
    }
  }
  Num res1 = 0, res2 = 0;
  for (const auto [x, y, p] : map.iter()) {
    if (p.h == 9) {
      res1 += p.origins.size();
      res2 += p.ways;
    }
  }
  return {res1, res2};
}

// ------------------------------------------------------------------------ //

int
main() {
  std::vector<std::string> lines;
  std::string line;
  while (std::cin >> line) {
    lines.emplace_back(line);
  }
  auto map = PlaceMap(lines, {}, [](auto c) { return Place{c - '0', {}, 0}; });

  auto [res1, res2] = solve(map);
  std::cout << "1: " << res1 << "\n";
  std::cout << "2: " << res2 << "\n";

  return 0;
}
