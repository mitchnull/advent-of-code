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

using uint = uint32;

using Range = std::pair<uint, uint>;

static Range
parseRange(const std::string_view& sv) {
  auto mid = std::ranges::find(sv, '-');
  uint a, b;
  std::from_chars(sv.begin(), mid, a);
  std::from_chars(mid + 1, sv.end(), b);
  return {a, b};
}

static std::pair<Range, Range>
parse(const std::string& line) {
  auto mid = std::ranges::find(line, ',');
  return {parseRange({line.begin(), mid}), parseRange({mid + 1, line.end()})};
}

static bool
containsOneWay(Range outer, Range inner) {
  return outer.first <= inner.first && inner.second <= outer.second;
}

static bool
contains(Range a, Range b) {
  return containsOneWay(a, b) || containsOneWay(b, a);
}

int
main() {
  std::string line;
  uint res = 0;
  while (std::cin >> line) {
    auto ab = parse(line);
    if (contains(ab.first, ab.second)) {
      ++res;
    }
  }
  std::cout << res << "\n";
}
