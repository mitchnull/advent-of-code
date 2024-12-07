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

using uint8 = std::uint8_t;
using int8 = std::int8_t;
using uint32 = std::uint32_t;
using int32 = std::int32_t;
using uint64 = std::uint64_t;
using int64 = std::int64_t;

using uint = uint32;

static uint
v(char c) {
  return c - 'A' + 1;
}

static uint
score(char a, char x) {
  auto va = v(a);
  auto vx = v(x);
  return vx + ((vx - va + 4) % 3) * 3;
}

static uint
guess(char a, char x) {
  return 'A' + ((a - 'A') + (x - 'Y') + 3) % 3;
}

int
main() {
  char a, x;
  uint res = 0;
  while (std::cin >> a >> x) {
    res += score(a, guess(a, x));
  }
  std::cout << res << "\n";
  return 0;
}
