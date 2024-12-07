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

using uint8 = std::uint8_t;
using int8 = std::int8_t;
using uint32 = std::uint32_t;
using int32 = std::int32_t;
using uint64 = std::uint64_t;
using int64 = std::int64_t;

using Num = int;

using Positions = std::vector<Num>;

static Num
calc(const Positions &positions, Num p) {
  Num res = 0;
  for (auto i: positions) {
    res += std::abs(i - p);
  }
  return res;
}

int
main() {
  std::cin >> std::noskipws;
  Num n;
  Num min = std::numeric_limits<Num>::max();
  Num max = std::numeric_limits<Num>::min();
  char c = ',';
  Positions positions;
  while (c == ',') {
    std::cin >> n >> c;
    positions.push_back(n);
    min = std::min(min, n);
    max = std::max(max, n);
  }
  Num currMin = std::numeric_limits<Num>::max();
  for (Num i = min; i <= max; ++i) {
    auto fuel = calc(positions, i);
    if (fuel < currMin) {
      currMin = fuel;
    }
  }
  std::cout << currMin << "\n";
  return 0;
}
