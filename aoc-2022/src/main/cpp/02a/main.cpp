#include <algorithm>
#include <bitset>
#include <cctype>
#include <charconv>
#include <deque>
#include <ios>
#include <iostream>
#include <limits>
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

static uint
v(char c) {
  switch (c) {
    case 'A': case 'X':
      return 1;
    case 'B': case 'Y':
      return 2;
    case 'C': case 'Z':
      return 3;
  }
  return 0;
}

static uint
score(char a, char x) {
  auto va = v(a);
  auto vx = v(x);
  return vx + ((vx - va + 4) % 3) * 3;
}


int
main() {
  char a, x;
  uint res = 0;
  while (std::cin >> a >> x) {
    res += score(a, x);
  }
  std::cout << res << "\n";
  return 0;
}
