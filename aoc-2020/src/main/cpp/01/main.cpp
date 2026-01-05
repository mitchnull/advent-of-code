#include "../utils.h"
#include <iterator>

using Num = int64_t;

static Num
solve1(const std::vector<Num> &v) {
  for (auto i = begin(v), e = end(v); i < e - 1; ++i) {
    for (auto j = i + 1; j < e; ++j) {
      if (*i + *j == 2020) {
        return *i * *j;
      }
    }
  }
  return -1;
}

static Num
solve2(const std::vector<Num> &v) {
  for (auto i = begin(v), e = end(v); i < e - 2; ++i) {
    for (auto j = i + 1; j < e - 1; ++j) {
      for (auto k = j + 1; k < e; ++k) {
        if (*i + *j + *k == 2020) {
          return *i * *j * *k;
        }
      }
    }
  }
  return -1;
}

/* ------------------------------------------------------------------------ */

int
main() {
  std::vector<Num> v;
  std::copy(std::istream_iterator<Num>(std::cin), std::istream_iterator<Num>(), std::back_inserter(v));
  println("1: {}", solve1(v));
  println("1: {}", solve2(v));

  return 0;
}
