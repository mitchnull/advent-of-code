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
d2(const Pos3& a, const Pos3& b) {
  Num res{};
  for (int i = 0; i < a.size(); ++i) {
    res += (a[i] - b[i]) * (a[i] - b[i]);
  }
  return res;
}


static Num
solve1(const Groups& groups) {
  auto groupCounts = std::unordered_map<int, Num>{};

  for (const auto& g : groups) {
    if (g != 0) {
      ++groupCounts[g];
    }
  }
  auto counts = std::vector<int>{};
  for (const auto& gc: groupCounts) {
    counts.push_back(gc.second);
  }

  std::sort(counts.begin(), counts.end(), std::greater<>{});

  return std::accumulate(counts.begin(), counts.begin() + 3, Num{1}, std::multiplies<>{});
}

static int
isSingleGroup(const Groups& groups) {
  auto g = groups.front();
  for (const auto& gg : groups) {
    if (gg != g) {
      return false;
    }
  }
  return true;
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
  const auto N = (v.size() < 100 ? 10UZ : 1000UZ);

  auto tups = Tups{};
  for (int i = 0; i < v.size(); ++i) {
    for (int j = i + 1; j < v.size(); ++j) {
      tups.emplace_back(i, j, d2(v[i], v[j]));
    }
  }
  std::sort(tups.begin(), tups.end(), [](const auto& a, const auto& b) { return a.d2 < b.d2; });

  auto groups = Groups(v.size(), 0);
  Num res1{}, res2{};
  int g = 0;
  for (auto i = 0; i < tups.size(); ++i) {
    const auto& t = tups[i];
    if (i == N) {
      res1 = solve1(groups);
    }
    auto g1 = groups[t.a];
    auto g2 = groups[t.b];
    if (g1 == 0 && g2 == 0) {
      groups[t.a] = groups[t.b] = ++g;
    } else if (g1 == 0) {
      groups[t.a] = g2;
    } else if (g2 ==0) {
      groups[t.b] = g1;
    } else {
      for (auto& og : groups) {
        if (og == g2) {
          og = g1;
        }
      }
    }
    if (i > N && isSingleGroup(groups)) {
      res2 = v[t.a][0] * v[t.b][0];
      break;
    }
  }

  println("1: {}", res1);
  println("2: {}", res2);

  return 0;
}
