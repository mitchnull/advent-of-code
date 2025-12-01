#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>
#include <print>
#include <ranges>

using Num = std::int32_t;

int
main() {
  const Num N = 100;
  char c;
  Num n, res1{}, res2{}, curr{50};
  while (std::cin >> c >> n) {
    Num d = (c == 'R' ? 1 : -1);
    while (n--) {
      res2 += !(curr = (curr + d + N) % N);
    }
    res1 += !curr;
  }
  std::println("1: {}", res1);
  std::println("2: {}", res2);

  return 0;
}
