#include "../utils.h"
#include <queue>
#include <unordered_map>

using Num = int;
using Board = Grid<>;

static const auto Costs = std::vector<Num>{1, 10, 100, 1000};
static const auto HallwayPlaces =
    std::vector<Pos>{Pos{1, 1}, Pos{2, 1}, Pos{4, 1}, Pos{6, 1}, Pos{8, 1}, Pos{10, 1}, Pos{10, 1}, Pos{11, 1}};
static const auto HomePlaces = std::vector<std::vector<Pos>>{
    {Pos{3, 3}, Pos{3, 2}}, {Pos{5, 3}, Pos{5, 2}}, {Pos{7, 3}, Pos{7, 2}}, {Pos{9, 3}, Pos{9, 2}}};

using Places = std::array<Pos, 8>;
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
using Prevs = std::unordered_map<Places, Places>;

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
  i = (i / 2) * 2;
  if (places[i] > places[i + 1]) {
    std::swap(places[i], places[i + 1]);
  }
}

static bool
tryMove(Queue &q, Dists &dists, Num dist, Prevs &prevs, const Places &p, int i, Pos to) {
  auto cost = moveCost(p, i, to);
  if (cost > 0) {
    auto np = p;
    np[i] = to;
    sort(np, i);
    auto nd = dist + cost;
    if (auto it = dists.find(np); it == dists.end() || nd < it->second) {
      dists[np] = nd;
      prevs[np] = p;
      println("@@@ move {}: {} -> {}, cost: {}", static_cast<char>('A' + (i / 2)), p, np, cost);
      q.emplace(np, nd);
      return true;
    }
  }
  return false;
}

static void
print(const Places &p, const Dists &dists, Board b) {
  for (int i = 0; i < p.size(); ++i) {
    b[p[i]] = 'A' + (i / 2);
  }
  println("@@@ cost: {}\n{}", dists.at(p), b);
}

static Num
solve1(const Places &start, const Board &b) {
  // Dijkstra
  Dists dists;
  Prevs prevs;

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
      print(p, dists, b);
      for (auto it = prevs.find(p); it != prevs.end(); it = prevs.find(p)) {
        p = it->second;
        print(p, dists, b);
      }
      return dist;
    }
    for (int i = 0; i < p.size(); ++i) {
      if (isHallway(p[i])) {
        if (!tryMove(q, dists, dist, prevs, p, i, HomePlaces[i / 2].front())) {
          if (at(p, HomePlaces[i / 2].front()) == i / 2) {
            tryMove(q, dists, dist, prevs, p, i, HomePlaces[i / 2].back());
          }
        }
      } else {
        for (auto np : HallwayPlaces) {
          tryMove(q, dists, dist, prevs, p, i, np);
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

  Places p = {};
  for (auto [x, y, v] : board.iter()) {
    if ('A' <= v && v <= 'D') {
      int i = v - 'A';
      if (p[i * 2] == Pos{}) {
        p[i * 2] = Pos{x, y};
      } else {
        p[i * 2 + 1] = Pos{x, y};
        sort(p, i);
      }
      board[x, y] = '.';
    }
  }

  { // @@@
    Board b = board;
    for (int i = 0; i < HallwayPlaces.size(); ++i) {
      b[HallwayPlaces[i]] = '0' + i;
    }
    for (int i = 0; i < HomePlaces.size(); ++i) {
      for (auto p : HomePlaces[i]) {
        b[p] = 'A' + i;
      }
    }
    println("@@@ places:\n{}", b);
  }

  println("{}", board);
  println("{}", p);
  println("1: {}", solve1(p, board));
  return 0;
}
