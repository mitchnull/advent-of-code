#include <ios>
#include <iostream>
#include <cctype>
#include <cstdint>
#include <limits>
#include <deque>
#include <numeric>
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

using Nums = std::vector<int>;

static bool
check(const std::string& line) {
  auto ss = std::stringstream(line);
  auto nums = Nums(std::istream_iterator<int>(ss), std::istream_iterator<int>());
  std::adjacent_difference(nums.begin(), nums.end(), nums.begin());
  return std::find_if(nums.begin() + 1, nums.end(), [](auto a) { return a <= 0 || 3 < a; }) == nums.end() ||
    std::find_if(nums.begin() + 1, nums.end(), [](auto a) { return a < -3 || 0 <= a; }) == nums.end();
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
