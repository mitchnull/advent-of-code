#include <iostream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <ranges>

namespace views = std::views;
namespace ranges = std::ranges;

using Num = std::uint64_t;
using Nums = std::vector<Num>;
using Counts = std::unordered_map<Num, Num>;

static Num
solve(Nums nums, int rounds) {
  auto counts = nums | views::transform([](auto n) { return std::make_pair(n, 1UL); }) | ranges::to<Counts>();
  for (auto i = 0; i < rounds; ++i) {
    Counts next;
    for (auto [n, c] : counts) {
      if (n == 0) {
        next[1] += c;
      } else if (int d; (d = std::log10(n) + 1) % 2 == 0) {
        Num m = std::pow(10, d / 2);
        next[n / m] += c;
        next[n % m] += c;
      } else {
        next[n * 2024] += c;
      }
    }
    std::swap(counts, next);
  }
  return std::reduce(counts.begin(), counts.end(), 0UL, [](auto a, auto b) { return a + b.second; });
}

int
main() {
  std::vector<Num> nums;
  Num n;
  while (std::cin >> n) {
    nums.push_back(n);
  }
  
  std::cout << "1: " << solve(nums, 25) << "\n";
  std::cout << "2: " << solve(nums, 75) << "\n";
  return 0;
}
