#include <ios>
#include <iostream>
#include <cctype>
#include <cstdint>
#include <limits>
#include <deque>
#include <string>
#include <vector>
#include <bitset>
#include <algorithm>
#include <ranges>
#include <charconv>
#include <sstream>
#include <iterator>

using uint8 = std::uint8_t;
using int8 = std::int8_t;
using uint32 = std::uint32_t;
using int32 = std::int32_t;
using uint64 = std::uint64_t;
using int64 = std::int64_t;

using uint = uint32;

using Nums = std::vector<int>;

template <typename T>
static int sgn(T v) {
  return (v < 0) ? -1 : (v > 0);
}

static int
get(const Nums& nums, uint i, uint dampened) {
  return nums[i + (i >= dampened)];
}

static bool
check(const Nums& nums, uint dampened) {
  int ls = 0;
  int last = get(nums, 0, dampened);
  for (uint i = 1; i < nums.size() - (dampened < nums.size()); ++i) {
    int num = get(nums, i, dampened);
    int diff = num - last;
    last = num;
    if (ls == 0) {
      ls = sgn(diff);
    }
    int cs = sgn(diff);
    if (cs == 0 || cs != ls || std::abs(diff) > 3) {
      return false;
    }
  }
  return true;
}

static bool
checkWithDampen(const std::string& line) {
  auto ss = std::stringstream(line);
  auto nums = Nums(std::istream_iterator<int>(ss), std::istream_iterator<int>());
  for (uint d = 0; d <= nums.size(); ++d) {
    if (check(nums, d)) {
      return true;
    }
  }
  return false;
}

int
main() {
  std::string line;

  int sum = 0;
  while (std::getline(std::cin, line)) {
    sum += checkWithDampen(line);
  }
  std::cout << sum << "\n";
  return 0;
}
