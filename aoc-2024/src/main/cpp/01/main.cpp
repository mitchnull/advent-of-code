#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

using uint = std::uint32_t;

int
main() {
  std::vector<int> as, bs;
  int a, b;
  while (std::cin >> a >> b) {
    as.push_back(a);
    bs.push_back(b);
  }
  
  std::sort(as.begin(), as.end());
  std::sort(bs.begin(), bs.end());
  int res1 = 0;
  for (uint i = 0; i < as.size(); ++i) {
    res1 += std::abs(as[i] - bs[i]);
  }
  std::cout << "1: " << res1 << "\n";

  // ------------------------------------------------------------------------ //

  int res2 = 0;
  for (const auto &a : as) {
    res2 += a * std::count(bs.begin(), bs.end(), a);
  }
  std::cout << "2: " << res2 << "\n";
  return 0;
}
