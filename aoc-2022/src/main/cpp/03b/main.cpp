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

using Set = std::vector<char>;

static uint
prio(char c) {
  return c > 'Z' ? c - 'a' + 1 : c - 'A' + 27;
}

static Set
sort_uniq(std::string s) {
  std::sort(s.begin(), s.end());
  return Set{s.begin(), std::unique(s.begin(), s.end())};
}

static Set
intersect(const Set &a, const Set &b) {
  Set res;
  std::ranges::set_intersection(a, b, std::back_inserter(res));
  return res;
}

static uint
check(std::vector<std::string> group) {
  std::vector<Set> sets;
  std::transform(group.begin(), group.end(), std::back_inserter(sets), sort_uniq);
  auto badge = std::accumulate(sets.begin() + 1, sets.end(), sets.front(), intersect);
  uint res = 0;
  for (auto c : badge) {
    res += prio(c);
  }
  return res;
}

static uint
betterCheck(std::vector<std::string> group) {
  auto sets = group | std::views::transform(sort_uniq);
  Set badge = std::accumulate(sets.begin() + 1, sets.end(), sets.front(), intersect);
  return std::transform_reduce(badge.begin(), badge.end(), 0, std::plus<>{}, prio);
}

static auto
readGroup() {
  std::vector<std::string> res;
  std::string line;
  for (uint i = 0; i < 3 && std::cin >> line; ++i) {
    res.emplace_back(std::move(line));
  }
  return res;
}

int
main() {
  std::vector<std::string> group;
  uint res = 0;
  while (!(group = readGroup()).empty()) {
    res += betterCheck(std::move(group));
  }
  std::cout << res << "\n";
}
