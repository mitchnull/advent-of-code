#include "../utils.h"
#include <algorithm>
#include <iterator>
#include <numeric>

using Num = int64_t;
using Nums = std::vector<Num>;

template <typename It>
static bool
check(It b, It e, Num n) {
  for (auto i = b; i < e - 1; ++i) {
    for (auto j = i + 1; j < e; ++j) {
      if (n == *i + *j) {
        return true;
      }
    }
  }
  return false;
}

static Num
solve1(const Nums &nums) {
  int M = nums.size() < 25 ? 5 : 25;
  for (auto i = nums.begin() + M, e = nums.end(); i < e; ++i) {
    if (!check(i - M, i, *i)) {
      return *i;
    }
  }
  return -1;
}

static Num
solve2(const Nums &nums, Num n) {
  for (auto i = nums.begin(), e = nums.end(); i < e - 2; ++i) {
    for (auto j = i + 2; j < e; ++j) {
      if (std::reduce(i, j) == n) {
        auto [minIt, maxIt] = std::minmax_element(i, j);
        return *minIt + *maxIt;
      }
    }
  }
  return -1;
}

/* ------------------------------------------------------------------------ */

int
main() {
  Nums nums;
  std::copy(std::istream_iterator<Num>(std::cin), std::istream_iterator<Num>(), std::back_inserter(nums));
  Num res1 = solve1(nums);
  Num res2 = solve2(nums, res1);
  println("1: {}", res1);
  println("1: {}", res2);

  return 0;
}
