#include <cstdint>
#include <iostream>
#include <print>
#include <algorithm>

using Num = std::int64_t;
using std::println;

static Num
jolts(auto b, auto e, auto e1, int r) {
  if (r == 0) {
    return 0;
  }
  if (b == e1) {
    return -1;
  }
  auto d1 = std::max_element(b, e1);
  auto n = d1 + 1;
  Num v2 = jolts(n, e, e, r - 1);
  if (v2 >= 0) {
    return (*d1 - '0') * std::pow(10, r - 1) + v2;
  }
  return jolts(b, e, e1 - 1, r);
}

/* ------------------------------------------------------------------------ */

int
main() {
  Num res1{}, res2{};
  std::string s;
  while (std::cin >> s) {
    res1 += jolts(s.begin(), s.end(), s.end(), 2);
    res2 += jolts(s.begin(), s.end(), s.end(), 12);
  }
  println("1: {}", res1);
  println("2: {}", res2);

  return 0;
}
