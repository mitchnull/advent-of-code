#include <ios>
#include <iostream>
#include <cctype>
#include <cstdint>
#include <limits>
#include <deque>
#include <string>
#include <vector>
#include <map>
#include <bitset>
#include <algorithm>
#include <ranges>
#include <unordered_map>
#include <sstream>
#include <charconv>

using uint8 = std::uint8_t;
using int8 = std::int8_t;
using uint32 = std::uint32_t;
using int32 = std::int32_t;
using uint64 = std::uint64_t;
using int64 = std::int64_t;

using uint = uint32;

using Num = int64;

struct Range {
  Num b, e;

  bool
  contains(Num i) {
    return b <= i && i < e;
  }

  friend auto operator<=>(const Range&, const Range&) = default;
  friend bool operator<(const Range& range, Num i) {
    return range.e <= i;
  }
  friend bool operator<(Num i, const Range& range) {
    return i < range.b;
  }
};

using Ranges = std::map<Range, Range, std::less<>>;

static Num
map(const Ranges& ranges, Num i) {
  auto it = ranges.find(i);
  if (it == ranges.end()) {
    return i;
  }
  return it->second.b + (i - it->first.b);
}

static Num
map(const std::vector<Ranges>& ranges, Num i) {
  for (const auto& r: ranges) {
    i = map(r, i);
  }
  return i;
}

int
main() {
  std::string line, word;
  std::getline(std::cin, line);
  std::stringstream ss(line);
  ss >> word;
  std::vector<Range> seeds;
  Num b, len;
  while (ss >> b >> len) {
    seeds.emplace_back(b, b + len);
  }
  std::vector<Ranges> maps;
  Ranges ranges;
  while (std::getline(std::cin, line)) {
    if (line.empty() || !std::isdigit(line[0])) {
      if (!ranges.empty()) {
        maps.push_back(ranges);
        ranges.clear();
      }
      continue;
    }
    std::stringstream ss(line);
    Num src, dst, len;
    ss >> dst >> src >> len;
    ranges[Range{src, src + len}] = Range{dst, dst + len};
  }
  if (!ranges.empty()) {
    maps.push_back(ranges);
    ranges.clear();
  }
  
  Num min = std::numeric_limits<Num>::max();
  for (Range r : seeds) {
    for (Num i = r.b; i < r.e; ++i) {
      Num mapped = map(maps, i);
      min = std::min(mapped, min);
    }
  }
  std::cout << min << "\n";
  return 0;
}
