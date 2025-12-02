#include <cstdint>
#include <iostream>
#include <print>
#include <cmath>
#include <unordered_set>

using Num = std::int64_t;
using Nums = std::unordered_set<Num>;

int
static digits(Num n) {
  return static_cast<int>(std::log10(n)) + 1;
}

static Num
half(Num n) {
  return n / std::pow(10, (digits(n) + 1) / 2);
}

static Num
dup(Num h) {
  return h * std::pow(10, digits(h)) + h;
}

/* ------------------------------------------------------------------------ */

static Num
invalids1(Num a, Num b) {
  Num res{};
  Num h = half(a);
  for (Num n = dup(h); n <= b; n = dup(++h)) {
    if (n >= a) {
      res += n;
    }
  }
  return res;
}

/* ------------------------------------------------------------------------ */

static Num
rep(Num n, int r) {
  Num res = n;
  int d = digits(n);
  while (--r) {
    res = res * std::pow(10, d) + n;
  }
  return res;
}

static Num
invalids2(Num a, Num b) {
  Nums found{};
  Num res{};
  int da = digits(a);
  int db = digits(b);
  for (int pl = 1; pl <= db / 2; ++pl) {
    for (int r = std::max(da / pl, 2); r <= db / pl; ++r) {
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
          res += n;
        }
      }
    }
  }
  return res;
}

/* ------------------------------------------------------------------------ */

int
main() {
  char c;
  Num a, b;
  Num res1{}, res2{};

  while (std::cin >> a >> c >> b) {
    std::cin >> c;
    res1 += invalids1(a, b);
    res2 += invalids2(a, b);
  }
  std::println("1: {}", res1);
  std::println("2: {}", res2);

  return 0;
}
