#include "../utils.h"
#include <algorithm>
#include <iterator>

using Num = int64_t;
using Nums = std::vector<Num>;

static Num
solve1(const Nums &nums) {
  Num prev = 0;
  std::array<Num, 4> diffs = {};
  for (auto n : nums) {
    ++diffs[n - prev];
    prev = n;
  }
  return diffs[1] * diffs[3];
}

/* ------------------------------------------------------------------------ */

int
main() {
  Nums nums;
  std::copy(std::istream_iterator<Num>(std::cin), std::istream_iterator<Num>(), std::back_inserter(nums));
  ranges::sort(nums);
  nums.push_back(nums.back() + 3);
  println("1: {}", solve1(nums));

  return 0;
}
