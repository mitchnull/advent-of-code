#include <iostream>
#include <cstdint>
#include <cmath>
#include <unordered_set>
#include "../utils.h"

using Num = std::int64_t;
using Nums = std::unordered_set<Num>;

int
static digits(Num n) {
  return static_cast<int>(std::log10(n)) + 1;
}

static Num
rep(Num n, int r) {
  Num res = n;
  while (--r) {
    res = res * std::pow(10, digits(n)) + n;
  }
  return res;
}

static auto
invalids(Num a, Num b) {
  Nums found{};
  Num res1{}, res2{};
  int db = digits(b);
  for (int r = 2; r <= db; ++r) {
    for (int pl = 1; pl <= db / r; ++pl) {
      for (Num rn = std::pow(10, pl - 1), end = std::pow(10, pl); rn < end; ++rn) {
        Num n = rep(rn, r);
        if (n < a) {
          continue;
        }
        if (n > b) {
          break;
        }
        auto [it, inserted] = found.insert(n);
        if (inserted) {
          if (r == 2) {
            res1 += n;
          }
          res2 += n;
        }
      }
    }
  }
  return std::make_pair(res1, res2);
}

/* ------------------------------------------------------------------------ */

int
main() {
  char c;
  Num a, b;
  auto res = std::make_pair(Num{}, Num{});

  while (std::cin >> a >> c >> b) {
    std::cin >> c;
    res += invalids(a, b);
  }
  println("1: {}", res.first);
  println("2: {}", res.second);

  return 0;
}
