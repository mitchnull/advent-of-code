#include <ios>
#include <iostream>
#include <cctype>
#include <cstdint>
#include <limits>
#include <deque>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <bitset>
#include <algorithm>
#include <ranges>
#include <sstream>
#include <numeric>
#include <gmpxx.h>

using Num = mpz_class;

namespace ranges = std::ranges;
namespace views = ranges::views;


static const Num M = 1'000'000 - 1;

struct Pos {
  Num i, j;
  friend auto operator<=>(const Pos& a, const Pos& b) = default;
};

using V = std::vector<Pos>;

static void
expand(V& v, Num Pos::* proj) {
  std::sort(v.begin(), v.end(), [&](auto& a, auto& b) { return a.*proj < b.*proj; });
  Num k = 0;
  for (auto it = v.begin(), end = v.end(); it < end; ++it) {
    Num offset = ((*it).*proj - k) - 1;
    if (offset > 0) {
      offset *= M;
      for (auto jit = it; jit < end; ++jit) {
        (*jit).*proj += offset;
      }
    }
    k = (*it).*proj;
  }
}

static Num
d(const Pos& a, const Pos& b) {
  return abs(a.i - b.i) + abs(a.j - b.j);
}

int
main() {
  Num i =0;
  Num j = 0;

  V v;
  char c;
  while (std::cin >> std::noskipws >> c) {
    switch (c) {
      case '\n':
        ++i;
        j = 0;
        continue;
      case '#':
        v.emplace_back(i, j);
        break;
    }
    ++j;
  }
  expand(v, &Pos::i);
  expand(v, &Pos::j);

  Num sum = 0;
  for (int p = 0, e = v.size(); p < e; ++p) {
    for (int q = 0; q < p; ++q) {
      sum += d(v[p], v[q]);
    }
  }
  std::cout << sum << "\n";
  return 0;
}
