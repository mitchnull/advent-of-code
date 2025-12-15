#include <algorithm>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <numeric>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using Num = std::uint64_t;
using String = std::string;

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
  String changeLog;

  Buyer(Num seed, int rounds) : secrets{seed}, changeLog{} {
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

using Prices = std::unordered_map<String, int>;
using StringSet = std::unordered_set<String>;

static void
updatePrices(Prices &prices, const Buyer &buyer) {
  StringSet visited{};
  for (int i = 0; i < buyer.changeLog.size() - 4; ++i) {
    auto pattern = buyer.changeLog.substr(i, 4);
    if (visited.contains(pattern)) {
      continue;
    }
    visited.insert(pattern);
    prices[pattern] += buyer.secrets[i + 4] % 10;
  }
}

int
main() {
  std::vector<Num> seeds;
  std::copy(std::istream_iterator<Num>(std::cin), std::istream_iterator<Num>(), std::back_inserter(seeds));

  std::vector<Buyer> buyers;
  std::transform(seeds.begin(), seeds.end(), std::back_inserter(buyers), [](Num seed) { return Buyer(seed, 2000); });

  Num res1 = std::transform_reduce(
      buyers.begin(), buyers.end(), Num{}, std::plus<>(), [](auto b) { return b.secrets.back(); });
  std::cout << "1: " << res1 << std::endl;

  Prices prices;
  for (const auto &b : buyers) {
    updatePrices(prices, b);
  }
  auto it = std::max_element(prices.begin(), prices.end(), [](auto a, auto b) { return a.second < b.second; });
  std::cout << "2: " << it->second << std::endl;

  return 0;
}
