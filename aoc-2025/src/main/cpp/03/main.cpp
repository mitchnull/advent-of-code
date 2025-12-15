#include <algorithm>
#include <cstdint>
#include <iostream>
#include <print>

using Num = std::int64_t;
using std::println;

static Num
jolts(auto b, auto e, int r) {
  auto d = std::max_element(b, e - r + 1);
  return r == 0 ? 0 : (*d - '0') * std::pow(10, r - 1) + jolts(d + 1, e, r - 1);
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
