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

using Num = std::int64_t;

namespace ranges = std::ranges;
namespace views = ranges::views;

template <typename It>
static Num
prev(It b, It e) {
  auto first = *b;
  std::adjacent_difference(b, e, b);
  if (std::find_if(b + 1, e, [](const auto& v) { return v != 0; }) == e) {
    return first;
  }
  return first - prev(b + 1, e);
}

int
main() {
  std::string line;
  Num sum = 0;
  while (std::getline(std::cin, line)) {
    std::vector<Num> v;
    std::stringstream ss(line);
    Num n;
    while (ss >> n) {
      v.push_back(n);
    }
    sum += prev(v.begin(), v.end());
  }
  std::cout << sum << "\n";
  return 0;
}
