#include "../utils.h"
#include <algorithm>
#include <queue>
#include <unordered_map>

using Num = int;
using Board = Grid<>;

static const auto Costs = std::vector<Num>{1, 10, 100, 1000};
static const auto HallwayPlaces = std::vector<Pos>{{1, 0}, {2, 0}, {4, 0}, {6, 0}, {8, 0}, {10, 0}, {11, 0}};
static const auto ExtraLines = std::vector<string>{
    "  #D#C#B#A#  ",
    "  #D#B#A#C#  ",
};

using Dists = std::unordered_map<Board, Num>;

struct Node {
  Board b;
  Num dist;

  friend bool operator<(const Node &a, const Node &b) { return a.dist > b.dist; };
};

using Queue = std::priority_queue<Node>;

/* ------------------------------------------------------------------------ */

static bool
isFinal(const Board &b) {
  for (int y = 1; y < b.h(); ++y) {
    for (int i = 0; i < 4; ++i) {
      if (b[i * 2 + 3, y] != 'A' + i) {
        return false;
      }
    }
  }
  return true;
}

static bool
isAllHome(const Board &b, Pos p) {
  for (; p.y < b.h(); ++p.y) {
    if ((b[p] - 'A') * 2 + 3 != p.x) {
      return false;
    }
  }
  return true;
}

static bool
isHallway(Pos pos) {
  return pos.y == 0;
}

static Num
moveCost(const Board &b, Pos p, Pos to) {
  Num cost = Costs[b[p] - 'A'] * (std::abs(to.x - p.x) + std::abs(to.y - p.y));
  auto dx = sgn(to.x - p.x);
  if (isHallway(p)) {
    while ((p.x += dx) != to.x) {
      if (b[p] != '.') {
        return -1;
      }
    }
    while (p.y != to.y) {
      ++p.y;
      if (b[p] != '.') {
        return -1;
      }
    }
  } else {
    while (--p.y != to.y) {
      if (b[p] != '.') {
        return -1;
      }
    }
    while (p.x != to.x) {
      p.x += dx;
      if (b[p] != '.') {
        return -1;
      }
    }
  }
  return cost;
}

static bool
tryMove(Queue &q, Dists &dists, Num dist, const Board &b, Pos from, Pos to) {
  auto cost = moveCost(b, from, to);
  if (cost > 0) {
    Board nb = b;
    std::swap(nb[to], nb[from]);
    auto nd = dist + cost;
    if (auto it = dists.find(nb); it == dists.end() || nd < it->second) {
      dists[nb] = nd;
      q.emplace(nb, nd);
      return true;
    }
  }
  return false;
}

static Num
solve(const std::vector<string> &lines) {
  Board start{lines.begin() + 1, lines.end() - 1, ' '};
  // Dijkstra
  Dists dists;

  auto q = Queue{};
  q.emplace(start, 0);
  dists[start] = 0;

  while (!q.empty()) {
    auto [b, dist] = q.top();
    q.pop();
    if (dist != dists[b]) {
      continue;
    }
    if (isFinal(b)) {
      return dist;
    }
    for (auto hwp : HallwayPlaces) {
      char a = b[hwp];
      if (a != '.') {
        int i = a - 'A';
        Pos hp{i * 2 + 3, b.h() - 1};
        if (!tryMove(q, dists, dist, b, hwp, hp)) {
          while (hp.y > 1 && b[hp] == a) {
            --hp.y;
            if (tryMove(q, dists, dist, b, hwp, hp)) {
              break;
            }
          }
        }
      }
    }
    for (int i = 0; i < 4; ++i) {
      for (int y = 1; y < b.h(); ++y) {
        Pos hop{i * 2 + 3, y};
        if (b[hop] != '.') {
          if (!isAllHome(b, hop)) {
            for (auto hwp : HallwayPlaces) {
              tryMove(q, dists, dist, b, hop, hwp);
            }
          }
          break;
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
