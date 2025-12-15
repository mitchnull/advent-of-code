#include "../utils.h"
#include <algorithm>
#include <numeric>

using Num = int64_t;

using Pos3 = std::array<Num, 3>;
using Junctions = std::vector<Pos3>;
using Groups = std::vector<int>;

struct Tup {
  int a, b;
  Num d2;
};
using Tups = std::vector<Tup>;

static Num
d2(const Pos3 &a, const Pos3 &b) {
  Num res{};
  for (int i = 0; i < a.size(); ++i) {
    res += (a[i] - b[i]) * (a[i] - b[i]);
  }
  return res;
}

static Num
solve1(const Groups &groups) {
  auto groupCounts = std::unordered_map<int, Num>{};
  for (const auto &g : groups) {
    if (g != 0) {
      ++groupCounts[g];
    }
  }
  auto counts = groupCounts | views::transform([](auto e) { return e.second; }) | ranges::to<std::vector<int>>();
  std::partial_sort(counts.begin(), counts.begin() + 3, counts.end(), std::greater<>{});
  return std::reduce(counts.begin(), counts.begin() + 3, Num{1}, std::multiplies<>{});
}

/* ------------------------------------------------------------------------ */

int
main() {
  char c;
  Num x, y, z;
  auto v = Junctions{};
  while (std::cin >> x >> c >> y >> c >> z) {
    v.push_back({x, y, z});
  }
  const int N = (v.size() < 100 ? 10 : 1000);

  auto tups = Tups{};
  for (int i = 0; i < v.size(); ++i) {
    for (int j = i + 1; j < v.size(); ++j) {
      tups.emplace_back(i, j, d2(v[i], v[j]));
    }
  }
  auto comp = [](const auto &a, const auto &b) { return a.d2 > b.d2; };
  std::make_heap(tups.begin(), tups.end(), comp);

  auto groups = Groups(v.size(), 0);
  Num res1{}, res2{};
  int g = 0;
  int numGroups = v.size();
  for (int i = 0, e = tups.size(); i < e; ++i) {
    std::pop_heap(tups.begin(), tups.end(), comp);
    auto t = tups.back();
    tups.pop_back();
    if (i == N) {
      res1 = solve1(groups);
    }
    auto g1 = groups[t.a];
    auto g2 = groups[t.b];
    if (g1 == 0 && g2 == 0) {
      groups[t.a] = groups[t.b] = ++g;
    } else if (g1 == 0) {
      groups[t.a] = g2;
    } else if (g2 == 0) {
      groups[t.b] = g1;
    } else if (g1 == g2) {
      continue;
    } else {
      std::replace(groups.begin(), groups.end(), g1, g2);
    }
    if (--numGroups == 1) {
      res2 = v[t.a][0] * v[t.b][0];
      break;
    }
  }
  println("1: {}", res1);
  println("2: {}", res2);

  return 0;
}
