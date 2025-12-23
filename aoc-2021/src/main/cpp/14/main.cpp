#include "../utils.h"
#include <algorithm>
#include <unordered_map>

using Pair = std::pair<char, char>;
using Num = int64_t;
using PT = std::unordered_map<Pair, char>;
using Hist = std::unordered_map<char, Num>;
using Counts = std::unordered_map<Pair, Num>;

static Num
result(const Counts &counts, string poly) {
  Hist h;
  for (auto e : counts) {
    h[e.first.first] += e.second;
    h[e.first.second] += e.second;
  }
  for (auto &he : h) {
    if (he.first == poly.front() || he.first == poly.back()) {
      he.second = (he.second / 2) + 1;
    } else {
      he.second = (he.second + 1) / 2;
    }
  }
  auto [minIt, maxIt] = std::minmax_element(h.begin(), h.end(), [](auto a, auto b) { return a.second < b.second; });
  return maxIt->second - minIt->second;
}

static std::pair<Num, Num>
solve(const PT &pt, string poly) {
  Num res1 = 0;
  Counts counts;
  for (auto it = poly.begin(); it != poly.end() - 1; ++it) {
    ++counts[{*it, *(it + 1)}];
  }
  for (int i = 0; i < 40; ++i) {
    if (i == 10) {
      res1 = result(counts, poly);
    }
    Counts next;
    for (auto e : counts) {
      char ins = pt.at(e.first);
      next[{e.first.first, ins}] += e.second;
      next[{ins, e.first.second}] += e.second;
    }
    counts = std::move(next);
  }
  return {res1, result(counts, poly)};
};

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
  auto [res1, res2] = solve(pt, poly);
  println("1: {}", res1);
  println("2: {}", res2);

  return 0;
}
