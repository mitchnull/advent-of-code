#include <iostream>
#include <cstdint>
#include <print>
#include <algorithm>

using Num = std::int64_t;
using std::println;

static Num
jolts(auto b, auto e, int r) {
  Num res = 0;
  while (r-- > 0) {
    b = std::max_element(b, e - r);
    res = 10 * res + (*b++ - '0');
  }
  return res;
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
