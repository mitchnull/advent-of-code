#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <unordered_map>

using Num = std::uint64_t;
using Counts = std::unordered_map<Num, Num>;

static Num
solve(Counts counts, int rounds) {
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
  Counts counts;
  Num n;
  while (std::cin >> n) {
    ++counts[n];
  }

  std::cout << "1: " << solve(counts, 25) << "\n";
  std::cout << "2: " << solve(counts, 75) << "\n";
  return 0;
}
