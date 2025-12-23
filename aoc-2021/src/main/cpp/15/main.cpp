#include "../utils.h"
#include <queue>

using Num = int;
using Map = Grid<>;
using Prevs = Grid<Pos>;
using Dists = Grid<Num>;

struct Node {
  Pos p;
  Num dist;

  friend bool operator<(const Node &a, const Node &b) { return a.dist > b.dist; };
};

using Queue = std::priority_queue<Node>;

static Num
solve1(const Map &m) {
  // Dijkstra
  const auto src = Pos{0, 0};
  const auto dst = Pos(m.w() - 1, m.h() - 1);
  auto prevs = Prevs(m.w(), m.h());
  auto dists = Dists(m.w(), m.h(), std::numeric_limits<Num>::max());

  auto q = Queue{};
  q.emplace(src, 0);
  dists[src] = 0;

  while (!q.empty()) {
    auto [p, dist] = q.top();
    q.pop();
    if (dist != dists[p]) {
      continue;
    }
    if (p == dst) {
      return dist;
    }
    for (Dir dir : DIRS) {
      auto np = p + dir;
      auto npd = m[np];
      if (!npd) {
        continue;
      }
      auto nd = dist + npd;
      if (nd < dists[np]) {
        dists[np] = nd;
        prevs[np] = p;
        q.emplace(np, nd);
      }
    }
  }
  return -1;
}

static Num
solve2(const Map &m) {
  const int N = 5;
  Map large = Map(m.w() * 5, m.h() * 5);
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      for (int y = 0; y < m.h(); ++y) {
        for (int x = 0; x < m.w(); ++x) {
          large[j * m.w() + x, i * m.h() + y] = (m[x, y] + i + j - 1) % 9 + 1;
        }
      }
    }
  }
  return solve1(large);
}

/* ------------------------------------------------------------------------ */

int
main() {
  Map m = Map::read(std::cin, 0, [](auto c) { return c - '0'; });

  println("1: {}", solve1(m));
  println("2: {}", solve2(m));

  return 0;
}
