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
#include <sstream>

using uint8 = std::uint8_t;
using int8 = std::int8_t;
using uint32 = std::uint32_t;
using int32 = std::int32_t;
using uint64 = std::uint64_t;
using int64 = std::int64_t;

using uint = uint32;

template <typename T>
static int sgn(T v) {
  return (v < 0) ? -1 : (v > 0);
}

static bool
check(const std::string& line) {
  std::stringstream ss(line);
  int num = 0;
  int ls = 0;
  int last;
  ss >> last;
  while (ss >> num) {
    int diff = num - last;
    last = num;
    if (ls == 0) {
      ls = sgn(diff);
    }
    int cs = sgn(diff);
    if (cs == 0 || cs != ls || std::abs(diff) > 3) {
      return false;
    }
  }
  return true;
}

int
main() {
  std::string line;

  int sum = 0;
  while (std::getline(std::cin, line)) {
    sum += check(line);
  }
  std::cout << sum << "\n";
  return 0;
}
