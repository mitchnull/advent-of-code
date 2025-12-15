#include <cstdint>
#include <iostream>
#include <print>

using Num = std::int32_t;

int
main() {
  constexpr const Num N = 100;
  char c;
  Num n, res1{}, res2{}, curr{50};

  while (std::cin >> c >> n) {
    res2 += n / N;
    n %= N;
    Num r = c == 'R' ? n : -n;
    Num next = (curr + r + N) % N;
    res1 += !next;
    res2 += (curr != 0) && (!next || (r > 0 && next < curr) || (r < 0 && next > curr));
    curr = next;
  }
  std::println("1: {}", res1);
  std::println("2: {}", res2);

  return 0;
}
