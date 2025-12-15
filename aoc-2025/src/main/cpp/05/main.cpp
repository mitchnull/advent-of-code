#include "../utils.h"
#include <algorithm>
#include <sstream>

using Num = uint64_t;
using Range = std::pair<Num, Num>;
using Ranges = std::vector<Range>;


static bool
isFresh(const Ranges& ranges, Num i) {
  for (const auto& r: ranges) {
    if (r.first <= i && i <= r.second) {
      return true;
    }
  }
  return false;
}

static Num
solve2(Ranges ranges) {
  Num res{};
  std::sort(ranges.begin(), ranges.end(), [](auto a, auto b) { return a.first < b.first; });
  Range mr = ranges.front();
  for (const auto& r: ranges) {
    if (r.first > mr.second) {
      res += mr.second - mr.first + 1;
      mr = r;
    } else {
      mr = {mr.first, std::max(mr.second, r.second)};
    }
  }
  return res + mr.second - mr.first + 1;
}

/* ------------------------------------------------------------------------ */

int
main() {
  Ranges ranges;
  std::string line;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      break;
    }
    std::istringstream in{line};
    Num a, b;
    char c;
    in >> a >> c >> b; 
    ranges.emplace_back(a, b);
  }
  Num res1{};
  Num i;
  while (std::cin >> i) {
    res1 += isFresh(ranges, i);
  }
  Num res2 = solve2(ranges);

  println("1: {}", res1);
  println("2: {}", res2);

  return 0;
}
