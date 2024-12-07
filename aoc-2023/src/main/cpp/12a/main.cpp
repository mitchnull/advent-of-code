#include <ios>
#include <iostream>
#include <cctype>
#include <cstdint>
#include <limits>
#include <deque>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <bitset>
#include <algorithm>
#include <ranges>
#include <sstream>
#include <numeric>

using Num = std::uint64_t;

namespace ranges = std::ranges;
namespace views = ranges::views;

using V = std::vector<uint>;

static bool
inc(V& gaps, const V& lens, uint len, uint maxLen) {
  uint i = gaps.size();
  while (i-- > 0) {
    ++gaps[i];
    if (std::accumulate(gaps.begin(), gaps.end(), len) <= maxLen) {
      return true;
    }
    for (uint j = i; j < gaps.size(); ++j) {
      gaps[j] = 1;
    }
  }
  return false;
}

static std::string
expand(const V& gaps, const V& lens, uint maxLen) {
  std::string res;
  for (auto git = gaps.begin(), lit = lens.begin(), end = gaps.end(); git != end; ++git, ++lit) {
    for (uint i = 0; i < *git; ++i) {
      res += '.';
    }
    for (uint i = 0; i < *lit; ++i) {
      res += '#';
    }
  }
  while (res.size() < maxLen) {
    res += '.';
  }
  return res;
}

static bool
check(const std::string& rec, const std::string exp) {
  for (auto rit = rec.begin(), eit = exp.begin(), end = rec.end(); rit != end; ++rit, ++eit) {
    if (*rit != *eit && *rit != '?') {
      return false;
    }
  }
  return true;
}

static Num
arrangements(const std::string& rec, const V& lens) {
  uint maxLen = rec.size();
  uint len = std::accumulate(lens.begin(), lens.end(), 0);
  V gaps(lens.size(), 1);
  gaps.front() = 0;
  Num res = 0;
  do {
    if (check(rec, expand(gaps, lens, maxLen))) {
      ++res;
    }
  } while (inc(gaps, lens, len, maxLen));
  return res;
}

int
main() {
  std::string rec, alt;
  Num sum = 0;
  while (std::cin >> rec >> alt) {
    std::replace(alt.begin(), alt.end(), ',', ' ');
    std::stringstream ss(alt);
    V lens;
    int n;
    while (ss >> n) {
      lens.push_back(n);
    }
    sum += arrangements(rec, lens);
  }
  std::cout << sum << "\n";
  return 0;
}
