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

int
main() {
  int sum = 0;
  std::string line;
  auto isDigit = [](char c) { return std::isdigit(c); };
  auto val = [](char c) { return c - '0'; };
  while (std::getline(std::cin, line)) {
    auto b = std::find_if(line.cbegin(), line.cend(), isDigit);
    auto e = std::find_if(line.crbegin(), line.crend(), isDigit);
    sum += 10 * val(*b) + val(*e);
  }
  std::cout << sum << "\n";
  return 0;
}
