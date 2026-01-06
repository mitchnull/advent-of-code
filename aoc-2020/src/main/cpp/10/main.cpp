#include "../utils.h"
#include <algorithm>
#include <iterator>
#include <numeric>

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

static Num
solve2(const Nums &nums) {
  auto r = Nums(nums.back() + 1, 0);
  r[0] = 1;
  for (auto n : nums) {
    r[n] = std::reduce(r.begin() + std::max(Num{0}, n - 3), r.begin() + n);
  }
  return r.back();
}

/* ------------------------------------------------------------------------ */

int
main() {
  Nums nums;
  std::copy(std::istream_iterator<Num>(std::cin), std::istream_iterator<Num>(), std::back_inserter(nums));
  ranges::sort(nums);
  nums.push_back(nums.back() + 3);
  println("1: {}", solve1(nums));
  println("1: {}", solve2(nums));

  return 0;
}
