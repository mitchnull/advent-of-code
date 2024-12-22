#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>
#include <iterator>

using Num = std::uint64_t;

static constexpr const Num MOD = 16777216UL;

static constexpr Num
nextSecret(Num n) {
  n = (n ^ (n * 64)) % MOD;
  n = (n ^ (n / 32)) % MOD;
  n = (n ^ (n * 2048)) % MOD;
  return n;
}

static constexpr char
toChar(int n) {
  return 'm' + n;
}

struct Buyer {
  std::vector<Num> secrets;
  std::string changeLog;

  Buyer (Num seed, int rounds) : secrets{seed}, changeLog{} {
    int last = seed % 10;
    for (int i = 0; i < rounds; ++i) {
      seed = nextSecret(seed);
      int curr = seed % 10;
      changeLog.push_back(toChar(curr - last));
      secrets.push_back(seed);
      last = curr;
    }
  }
};

static Num
monkey(const Buyer buyer, std::string_view pattern) {
  if (auto p = buyer.changeLog.find(pattern); p != std::string::npos) {
    return buyer.secrets[p + 4] % 10;
  }
  return 0;
}

int
main() {
  std::vector<Num> seeds;
  std::copy(std::istream_iterator<Num>(std::cin), std::istream_iterator<Num>(), std::back_inserter(seeds));

  std::vector<Buyer> buyers;
  std::transform(seeds.begin(), seeds.end(), std::back_inserter(buyers), [](Num seed) { return Buyer(seed, 2000); });

  Num res1 = std::transform_reduce(buyers.begin(), buyers.end(), Num{}, std::plus<>(), [](auto b) { return b.secrets.back(); });
  std::cout << "1: " << res1 << std::endl;

  Num res2 = 0;
  for (int a = -9; a <= 9; ++a) {
  for (int b = -9; b <= 9; ++b) {
  for (int c = -9; c <= 9; ++c) {
  for (int d = -9; d <= 9; ++d) {
    std::string pattern = {toChar(a), toChar(b), toChar(c), toChar(d)};
    res2 = std::max(res2, std::transform_reduce(buyers.begin(), buyers.end(), Num{}, std::plus<>(), [&pattern](auto& b) { return monkey(b, pattern); }));
  }}}}
  std::cout << "2: " << res2 << std::endl;

  return 0;
}
