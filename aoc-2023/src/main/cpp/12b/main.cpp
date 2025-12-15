#include <algorithm>
#include <bitset>
#include <cctype>
#include <cstdint>
#include <deque>
#include <ios>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <ranges>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using Num = std::uint64_t;
using size_t = std::size_t;

namespace ranges = std::ranges;
namespace views = ranges::views;

static constexpr const uint MUL = 5;

using V = std::vector<uint>;

static size_t
findStart(uint len, std::string_view rec, size_t pos) {
  while ((pos = rec.find_first_of("#?", pos)) != std::string::npos) {
    auto end = pos + len;
    if (end > rec.size() || (end < rec.size() && rec[end] == '#') || rec.find('.', pos) < end) {
      if (rec[pos++] == '#') {
        break;
      }
      continue;
    }
    return pos;
  }
  return std::string::npos;
}

static std::string_view
substr(std::string_view sv, size_t pos) {
  if (pos > sv.size()) {
    return {};
  }
  return sv.substr(pos);
}

template <typename It>
static Num
arrangements(std::string_view rec, It lensBegin, It lensEnd, std::map<std::pair<std::string_view, It>, Num>& cache) {
  auto key = make_pair(rec, lensBegin);
  auto cached = cache.find(key);
  if (cached != cache.end()) {
    return cached->second;
  }
  if (lensBegin == lensEnd) {
    if (rec.contains('#')) {
      return 0;
    }
    return 1;
  }
  Num res = 0;
  for (auto pos = findStart(*lensBegin, rec, 0); pos != std::string::npos; pos = findStart(*lensBegin, rec, pos)) {
    res += arrangements(substr(rec, pos + *lensBegin + 1), lensBegin + 1, lensEnd, cache);
    if (rec[pos++] == '#') {
      break;
    }
  }
  cache[key] = res;
  return res;
}

template <typename It>
static Num
arrangements(std::string_view rec, It lensBegin, It lensEnd) {
  std::map<std::pair<std::string_view, It>, Num> cache;
  return arrangements(rec, lensBegin, lensEnd, cache);
}

static std::string
mul(std::string_view str, int count, char sep) {
  std::string res{str};
  while (--count > 0) {
    res += sep;
    res += str;
  }
  return res;
}

int
main() {
  std::string rec, alt;
  Num sum = 0;
  while (std::cin >> rec >> alt) {
    rec = mul(rec, MUL, '?');
    alt = mul(alt, MUL, ',');
    std::replace(alt.begin(), alt.end(), ',', ' ');
    std::stringstream ss(alt);
    V lens;
    int n;
    while (ss >> n) {
      lens.push_back(n);
    }
    sum += arrangements(rec, lens.begin(), lens.end());
  }
  std::cout << sum << "\n";
  return 0;
}
