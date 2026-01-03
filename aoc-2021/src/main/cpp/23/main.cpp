#include "../utils.h"
#include <algorithm>
#include <queue>
#include <unordered_map>

using Num = int;
using Board = Grid<>;

static const auto Costs = std::vector<Num>{1, 10, 100, 1000};
static const auto HallwayPlaces =
    std::vector<Pos>{Pos{1, 1}, Pos{2, 1}, Pos{4, 1}, Pos{6, 1}, Pos{8, 1}, Pos{10, 1}, Pos{10, 1}, Pos{11, 1}};
static const auto HomePlaces = std::vector<std::vector<Pos>>{
    {Pos{3, 2}, Pos{3, 3}}, {Pos{5, 2}, Pos{5, 3}}, {Pos{7, 2}, Pos{7, 3}}, {Pos{9, 2}, Pos{9, 3}}};
// #D#C#B#A#
// #D#B#A#C#
static const auto ExtraStartPlaces =
    std::vector<std::vector<Pos>>{{{7, 4}, {9, 3}}, {{5, 4}, {7, 3}}, {{5, 3}, {9, 4}}, {{3, 3}, {3, 4}}};

using Places = std::vector<Pos>;
template <>
struct std::hash<Places> {
  std::size_t operator()(const Places &p) const {
    auto res = std::hash<Pos>{}(p.front());
    for (int i = 1; i < p.size(); ++i) {
      res = hashCombine(res, p[i]);
    };
    return res;
  }
};

using Dists = std::unordered_map<Places, Num>;

struct Node {
  Places p;
  Num dist;

  friend bool operator<(const Node &a, const Node &b) { return a.dist > b.dist; };
};

using Queue = std::priority_queue<Node>;

static bool
isHome(const Places &places, int i) {
  const auto &h = HomePlaces[i / 2];
  return std::find(begin(h), end(h), places[i]) != h.end();
}

static bool
isFinal(const Places &places) {
  for (int i = 0; i < places.size(); ++i) {
    if (!isHome(places, i)) {
      return false;
    }
  }
  return true;
}

static bool
isHallway(Pos pos) {
  return pos.y == 1;
}

static int
at(const Places &places, Pos pos) {
  auto it = std::find(begin(places), end(places), pos);
  if (it == places.end()) {
    return -1;
  }
  return (it - places.begin()) / 2;
}

static Num
moveCost(const Places &places, int i, Pos to) {
  auto p = places[i];
  Num cost = Costs[i / 2] * (std::abs(to.x - p.x) + std::abs(to.y - p.y));
  auto dx = sgn(to.x - p.x);
  if (isHallway(p)) {
    while ((p.x += dx) != to.x) {
      if (at(places, p) != -1) {
        return -1;
      }
    }
    while (p.y != to.y) {
      ++p.y;
      if (at(places, p) != -1) {
        return -1;
      }
    }
  } else {
    while (--p.y != to.y) {
      if (at(places, p) != -1) {
        return -1;
      }
    }
    while (p.x != to.x) {
      p.x += dx;
      if (at(places, p) != -1) {
        return -1;
      }
    }
  }
  return cost;
}

static void
sort(Places &places, int i) {
  int hp = places.size() / 4;
  int i0 = (i / hp) * hp;
  std::sort(begin(places) + i0, begin(places) + i0 + hp);
}

static bool
tryMove(Queue &q, Dists &dists, Num dist, Places &p, int i, Pos to) {
  auto cost = moveCost(p, i, to);
  if (cost > 0) {
    auto np = p;
    np[i] = to;
    sort(np, i);
    auto nd = dist + cost;
    if (auto it = dists.find(np); it == dists.end() || nd < it->second) {
      dists[np] = nd;
      q.emplace(np, nd);
      return true;
    }
  }
  return false;
}

static Num
solve(const Places &start) {
  // Dijkstra
  Dists dists;

  auto q = Queue{};
  q.emplace(start, 0);
  dists[start] = 0;

  while (!q.empty()) {
    auto [p, dist] = q.top();
    q.pop();
    if (dist != dists[p]) {
      continue;
    }
    if (isFinal(p)) {
      return dist;
    }
    for (int i = 0; i < p.size(); ++i) {
      if (isHallway(p[i])) {
        int hpi = i / (p.size() / 4);
        if (!tryMove(q, dists, dist, p, i, HomePlaces[hpi].back())) {
          for (int j = p.size() / 4 - 1; j > 0; --j) {
            if (at(p, HomePlaces[hpi][j]) != hpi || tryMove(q, dists, dist, p, i, HomePlaces[hpi][j - 1])) {
              break;
            }
          }
        }
      } else {
        for (auto np : HallwayPlaces) {
          tryMove(q, dists, dist, p, i, np);
        }
      }
    }
  }
  return -1;
}

/* ------------------------------------------------------------------------ */

int
main() {
  Board board = Board::read(std::cin, ' ');

  Places p{8};
  for (auto [x, y, v] : board.iter()) {
    if ('A' <= v && v <= 'D') {
      int i = v - 'A';
      if (p[i * 2] == Pos{}) {
        p[i * 2] = Pos{x, y};
      } else {
        p[i * 2 + 1] = Pos{x, y};
        sort(p, i);
      }
    }
  }

  println("1: {}", solve(p));

  for (int i = 0; i < 4; ++i) {
    p.insert(begin(p) + (4 * i + 1), begin(ExtraStartPlaces[i]), end(ExtraStartPlaces[i]));
    p[4 * i + 3].y += 2;
  }
  println("2: {}", solve(p));
  return 0;
}
