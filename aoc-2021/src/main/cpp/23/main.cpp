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
    {Pos{3, 2}, Pos{3, 3}, Pos{3, 4}, Pos{3, 5}},
    {Pos{5, 2}, Pos{5, 3}, Pos{5, 4}, Pos{5, 5}},
    {Pos{7, 2}, Pos{7, 3}, Pos{7, 4}, Pos{7, 5}},
    {Pos{9, 2}, Pos{9, 3}, Pos{9, 4}, Pos{9, 5}}};
static const auto ExtraLines = std::vector<string> {
"  #D#C#B#A#  ",
"  #D#B#A#C#  ",
};

using Places = std::array<std::vector<Pos>, 4>;
template <>
struct std::hash<Places> {
  size_t operator()(const Places &p) const {
    size_t res = 0;
    for (const auto &pp : p) {
      for (const auto &ppp : pp) {
        res = hashCombine(res, ppp);
      }
    }
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

/* ------------------------------------------------------------------------ */

static bool
isHome(const Places &p, int i, int j) {
  const auto &h = HomePlaces[i];
  return std::find(begin(h), end(h), p[i][j]) != h.end();
}

static bool
isFinal(const Places &places) {
  for (int i = 0; i < places.size(); ++i) {
    for (int j = 0; j < places[i].size(); ++j) {
      if (!isHome(places, i, j)) {
        return false;
      }
    }
  }
  return true;
}

static bool
isHallway(Pos pos) {
  return pos.y == 1;
}

static int
at(const Places &p, Pos pos) {
  for (int i = 0; i < p.size(); ++i) {
    if (std::find(begin(p[i]), end(p[i]), pos) != end(p[i])) {
      return i;
    }
  }
  return -1;
}

static Num
moveCost(const Places &places, int i, int j, Pos to) {
  auto p = places[i][j];
  Num cost = Costs[i] * (std::abs(to.x - p.x) + std::abs(to.y - p.y));
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
  std::sort(begin(places[i]), end(places[i]));
}

static bool
tryMove(Queue &q, Dists &dists, Num dist, Places &p, int i, int j, Pos to) {
  auto cost = moveCost(p, i, j, to);
  if (cost > 0) {
    auto np = p;
    np[i][j] = to;
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
solve(const std::vector<string> lines) {
  Places start;
  for (int y = 2; y < lines.size() - 1; ++y) {
    const auto &line = lines[y];
    for (int x = 3; x < 10; x += 2) {
      auto v = line[x];
      if ('A' <= v && v <= 'D') {
        int i = v - 'A';
        start[i].emplace_back(x, y);
      }
    }
  }
  for (int i = 0; i < start.size(); ++i) {
    sort(start, i);
  }
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
      int hp = p[i].size();
      for (int j = 0; j < hp; ++j) {
        if (isHallway(p[i][j])) {
          if (!tryMove(q, dists, dist, p, i, j, HomePlaces[i][hp - 1])) {
            for (int k = hp - 1; k > 0; --k) {
              if (at(p, HomePlaces[i][k]) != i || tryMove(q, dists, dist, p, i, j, HomePlaces[i][k - 1])) {
                break;
              }
            }
          }
        } else {
          for (auto np : HallwayPlaces) {
            tryMove(q, dists, dist, p, i, j, np);
          }
        }
      }
    }
  }
  return -1;
}

/* ------------------------------------------------------------------------ */

int
main() {
  std::vector<string> lines;
  string line;
  while (std::getline(std::cin, line)) {
    lines.push_back(line);
  }
  println("1: {}", solve(lines));
  lines.insert(lines.begin() + 3, begin(ExtraLines), end(ExtraLines));
  println("2: {}", solve(lines));
  return 0;
}
