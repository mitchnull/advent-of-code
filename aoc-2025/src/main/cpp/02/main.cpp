#include <cstdint>
#include <iostream>
#include <print>
#include <cmath>
#include <unordered_set>

using Num = std::int64_t;
using Nums = std::unordered_set<Num>;
using std::println;

int
static digits(Num n) {
  return static_cast<int>(std::log10(n)) + 1;
}

static Num
rep(Num n, int r) {
  Num res = n;
  int d = digits(n);
  while (--r) {
    res = res * std::pow(10, d) + n;
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
          res2 += n;
          if (r == 2) {
            res1 += n;
          }
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
  Num res1{}, res2{};

  while (std::cin >> a >> c >> b) {
    std::cin >> c;
    auto [r1, r2] = invalids(a, b);
    res1 += r1;
    res2 += r2;
  }
  println("1: {}", res1);
  println("2: {}", res2);

  return 0;
}
