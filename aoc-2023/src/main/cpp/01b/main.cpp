#include <algorithm>
#include <bitset>
#include <cctype>
#include <charconv>
#include <cstdint>
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

static const std::vector<std::string> digits = {
  "0", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };

static int
parseDigit(auto b, auto e) {
  if (b == e) {
    return -1;
  }
  if (std::isdigit(*b)) {
    return *b - '0';
  }
  for (uint i = 1; i < digits.size(); ++i) {
    if (std::string_view(b, e).starts_with(digits[i])) {
      return i;
    }
  }
  return -1;
}

int
main() {
  int sum = 0;
  std::string line;
  while (std::getline(std::cin, line)) {
    int tens = -1;
    for (auto it = line.begin(), end = line.end(); it != end && tens < 0; ++it) {
      tens = parseDigit(it, end);
    }
    int ones = -1;
    for (auto it = line.rbegin(), rend = line.rend(); it != rend && ones < 0; ++it) {
      ones = parseDigit(--it.base(), line.end());
    }
    if (tens < 0 || ones < 0) {
      std::cerr << "bad line: " << line << "\n";
      std::exit(-1);
    }
    sum += 10 * tens + ones;
  }
  std::cout << sum << "\n";
  return 0;
}
