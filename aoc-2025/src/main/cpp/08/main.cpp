#include "../utils.h"
#include <algorithm>
#include <numeric>

using Num = int64_t;

using Pos3 = std::array<Num, 3>;
using V = std::vector<Pos3>;
using P = std::pair<Pos3, Pos3>;
using PV = std::vector<P>;
using M = std::unordered_map<Pos3, Num>;

template <>
struct std::hash<Pos3> {
  std::size_t operator()(const Pos3& p) const {
    return hashCombine(hashCombine(p[0], p[1]), p[2]);
  }
};


static Num
d2(const Pos3& a, const Pos3& b) {
  Num res{};
  for (int i = 0; i < a.size(); ++i) {
    res += (a[i] - b[i]) * (a[i] - b[i]);
  }
  return res;
}

/* ------------------------------------------------------------------------ */

int
main() {
  char c;
  Num x, y, z;
  auto v = V{};
  auto ps = PV{};
  while (std::cin >> x >> c >> y >> c >> z) {
    v.push_back({x, y, z});
  }
  const auto n = (v.size() < 100 ? 10UZ : 1000UZ);

  for (int i = 0; i < v.size(); ++i) {
    for (int j = i + 1; j < v.size(); ++j) {
      ps.push_back({v[i], v[j]});
    }
  }
  std::sort(ps.begin(), ps.end(), [](const auto& a, const auto& b) { return d2(a.first, a.second) < d2(b.first, b.second); });

  auto m = M{};
  int g = 0;
  for (auto i = 0; i < n; ++i) {
    const auto& p = ps[i];
    auto g1 = m[p.first];
    auto g2 = m[p.second];
    if (g1 == 0 && g2 == 0) {
      m[p.first] = m[p.second] = ++g;
    } else if (g1 == 0) {
      m[p.first] = g2;
    } else if (g2 ==0) {
      m[p.second] = g1;
    } else {
      for (auto& mv : m) {
        if (mv.second == g2) {
          mv.second = g1;
        }
      }
    }
  }
  auto groupCounts = std::unordered_map<int, Num>{};

  for (const auto& pg : m) {
    ++groupCounts[pg.second];
  }
  auto counts = std::vector<Num>{};
  for (const auto& gc: groupCounts) {
    counts.push_back(gc.second);
  }

  std::sort(counts.begin(), counts.end(), std::greater<>{});

  Num res1 = std::accumulate(counts.begin(), counts.begin() + 3, Num{1}, std::multiplies<>{});

  println("1: {}", res1);
  // println("2: {}", 0);

  return 0;
}
