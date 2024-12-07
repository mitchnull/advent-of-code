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
prio(char c) {
  return c > 'Z'
    ? c - 'a' + 1
    : c - 'A' + 27;
}

static auto
sort_uniq(auto b, auto e) {
  std::sort(b, e);
  return std::unique(b, e);
}

static uint
check(std::string line) {
  auto b1 = line.begin();
  auto b2 = b1 + line.length() / 2;
  auto e1 = sort_uniq(b1, b2);
  auto e2 = sort_uniq(b2, line.end());
  auto v = std::vector<char>();
  std::set_intersection(b1, e1, b2, e2, std::back_inserter(v));
  uint res = 0;
  for (auto c : v) {
    res += prio(c);
  }
  return res;
}

int
main() {
  std::string line;
  uint res = 0;
  while (std::cin >> line) {
    res += check(std::move(line));
  }
  std::cout << res << "\n";
}
