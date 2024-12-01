#include <ios>
#include <iostream>
#include <cctype>
#include <cstdint>
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


int
main() {
  std::vector<int> as, bs;
  int a, b;
  while (std::cin >> a >> b) {
    as.push_back(a);
    bs.push_back(b);
  }
  int sum = 0;
  for (const auto &a : as) {
    sum += a * std::count(bs.begin(), bs.end(), a);
  }
  std::cout << sum << "\n";
  return 0;
}
