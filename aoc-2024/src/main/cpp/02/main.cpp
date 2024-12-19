#include <algorithm>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

using uint = std::uint32_t;
using Strings = std::vector<std::string>;
using Nums = std::vector<int>;

static Strings readLines() {
  Strings lines;
  std::string line;
  while (std::getline(std::cin, line)) {
    lines.emplace_back(line);
  }
  return lines;
}

static bool
check(const std::string& line) {
  auto ss = std::stringstream(line);
  auto nums = Nums(std::istream_iterator<int>(ss), std::istream_iterator<int>());
  std::adjacent_difference(nums.begin(), nums.end(), nums.begin());
  return std::find_if(nums.begin() + 1, nums.end(), [](auto a) { return a <= 0 || 3 < a; }) == nums.end() ||
    std::find_if(nums.begin() + 1, nums.end(), [](auto a) { return a < -3 || 0 <= a; }) == nums.end();
}

template <typename T>
static int sgn(T v) {
  return (v < 0) ? -1 : (v > 0);
}

static int
get(const Nums& nums, uint i, uint dampened) {
  return nums[i + (i >= dampened)];
}

static bool
checkDampened(const Nums& nums, uint dampened) {
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
    if (checkDampened(nums, d)) {
      return true;
    }
  }
  return false;
}

int
main() {
  Strings lines = readLines();

  int res1 = std::count_if(lines.begin(), lines.end(), check);
  std::cout << "1: " << res1 << "\n";

  // ------------------------------------------------------------------------ //

  int res2 = std::count_if(lines.begin(), lines.end(), checkWithDampen);
  std::cout << "2: " << res2 << "\n";
  return 0;
}
