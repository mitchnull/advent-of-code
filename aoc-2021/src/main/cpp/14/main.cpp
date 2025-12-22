#include "../utils.h"
#include <algorithm>
#include <unordered_map>

struct Pair {
  char f, s;
  friend auto operator<=>(const Pair &, const Pair &) = default;
};

template <>
struct std::hash<Pair> {
  std::size_t operator()(const Pair &p) const { return hashCombine(p.f, p.s); }
};

using Num = int64_t;
using PT = std::unordered_map<Pair, char>;
using Hist = std::unordered_map<char, int>;

static Num
solve1(const PT &pt, string poly) {
  for (int i = 0; i < 10; ++i) {
    for (auto it = poly.begin(); it != poly.end() - 1; ++it) {
      auto pp = pt.find({*it, *(it + 1)});
      if (pp != pt.end()) {
        it = poly.insert(it + 1, pp->second);
      }
    }
  }
  Hist h;
  for (char c : poly) {
    ++h[c];
  }
  auto [minIt, maxIt] = std::minmax_element(h.begin(), h.end(), [](auto a, auto b) { return a.second < b.second; });
  return maxIt->second - minIt->second;
}

/* ------------------------------------------------------------------------ */

int
main() {
  string poly, pair, str;
  char f, s, ins;
  PT pt;
  std::cin >> poly;
  while (std::cin >> f >> s >> str >> ins) {
    pt[{f, s}] = ins;
  }
  println("1: {}", solve1(pt, poly));
  return 0;
}
