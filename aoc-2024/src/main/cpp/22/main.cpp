#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>
#include <iterator>

using Num = std::uint64_t;

static constexpr const Num MOD = 16777216UL;

constexpr Num
secret(Num seed, int rounds) {
  for (int i = 0; i < rounds; ++i) {
    seed = (seed ^ (seed * 64)) % MOD;
    seed = (seed ^ (seed / 32)) % MOD;
    seed = (seed ^ (seed * 2048)) % MOD;
  }
  return seed;
}

int
main() {
  std::vector<Num> seeds;
  std::copy(std::istream_iterator<Num>(std::cin), std::istream_iterator<Num>(), std::back_inserter(seeds));


  Num res1 = std::transform_reduce(seeds.begin(), seeds.end(), Num{}, std::plus<>(), [](auto s) { return secret(s, 2000); });
  std::cout << "1: " << res1 << "\n";

  // ------------------------------------------------------------------------ //

  return 0;
}
