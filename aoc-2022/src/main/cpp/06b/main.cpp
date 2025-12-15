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

using Window = std::vector<char>;

constexpr const uint N = 14;

static bool
isUniq(const Window &w) {
  std::bitset<128> set{1};
  for (char c : w) {
    if (set.test(c)) {
      return false;
    }
    set.set(c);
  }
  return true;
}

int
main() {
  Window w(N, 0);
  uint i = 0;
  char c;
  while (std::cin >> c) {
    w[++i % N] = c;
    if (isUniq(w)) {
      std::cout << i << "\n";
      return 0;
    }
  }
  return 0;
}
