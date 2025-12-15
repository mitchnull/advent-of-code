#include "../utils.h"
#include <iostream>
#include <string>

/* ------------------------------------------------------------------------ */

using Num = std::int64_t;
using string = std::string;

struct Entry {
  Dir dir;
  Num steps;
};

using Entries = std::vector<Entry>;

static Dir
toDir(char c) {
  switch (c) {
    case 'U': case '3': return {0, -1};
    case 'D': case '1': return {0, 1};
    case 'L': case '2': return {-1, 0};
    case 'R': case '0': return {1, 0};
  }
  return {};
}

static Num
shoelace(const Entries& entries) {
  Num x = 0, y = 0;
  Num area = 0;
  Num len = 0;
  for (auto e: entries) {
    Num xx = x + (e.dir.dx * e.steps);
    Num yy = y + (e.dir.dy * e.steps);
    area += x * yy - xx * y;
    len += e.steps;
    x = xx;
    y = yy;
  }
  return (area / 2) + len / 2 + 1;
}

/* ------------------------------------------------------------------------ */

int
main() {
  char dir;
  int steps;
  string color;

  Entries entries1;
  Entries entries2;
  while (std::cin >> dir >> steps >> color) {
    entries1.emplace_back(toDir(dir), steps);
    entries2.emplace_back(toDir(color[7]), std::stoi(color.substr(2, 5), nullptr, 16));
  }
  auto res1 = shoelace(entries1);
  auto res2 = shoelace(entries2);

  std::cout << res1 << "\n";
  std::cout << res2 << "\n";
}
