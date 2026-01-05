#include "../utils.h"
#include <bitset>

using Num = int64_t;
using BS = std::bitset<26>;

/* ------------------------------------------------------------------------ */

int
main() {
  std::string line;
  BS bs1 = 0, bs2 = ~0;
  Num res1 = 0, res2 = 0;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      res1 += bs1.count();
      res2 += bs2.count();
      bs1 = 0;
      bs2 = ~0;

    } else {
      BS bss{};
      for (auto c : line) {
        bss.set(c - 'a');
      }
      bs1 |= bss;
      bs2 &= bss;
    }
  }
  res1 += bs1.count();
  res2 += bs2.count();
  println("1: {}", res1);
  println("2: {}", res2);

  return 0;
}
