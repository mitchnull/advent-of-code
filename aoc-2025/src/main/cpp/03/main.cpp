#include <iostream>
#include <cstdint>
#include <print>
#include <algorithm>

using Num = std::int64_t;
using std::println;

static Num
jolts(auto b, auto e, int r) {
  if (r == 0) {
    return 0;
  }
  auto d1 = std::max_element(b, e - r + 1);
  return (*d1 - '0') * std::pow(10, r - 1) + jolts(d1 + 1, e, r - 1);
}

/* ------------------------------------------------------------------------ */

int
main() {
  Num res1{}, res2{};
  std::string s;
  while (std::cin >> s) {
    res1 += jolts(s.begin(), s.end(), 2);
    res2 += jolts(s.begin(), s.end(), 12);
  }
  println("1: {}", res1);
  println("2: {}", res2);

  return 0;
}
