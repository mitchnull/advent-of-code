#include "../utils.h"
#include <queue>
#include <unordered_map>

using Num = int;
using Board = Grid<>;

static const auto Costs = std::vector<Num>{1, 10, 100, 1000};
static const auto HallwayPlaces = std::vector<Pos>{{0, 0}, {1, 0}, {3, 0}, {5, 0}, {7, 0}, {9, 0}, {10, 0}};
static const auto ExtraLines = std::vector<string>{
    "  #D#C#B#A#  ",
    "  #D#B#A#C#  ",
};

struct State {
  uint16_t hwDwellers;
  uint8_t hwEmpty;
  std::array<uint8_t, 4> homes;
  std::array<uint8_t, 4> homesFree;

  friend auto operator<=>(const State &, const State &) = default;
};

static constexpr const uint32_t
asUint(std::array<uint8_t, 4> bytes) {
  int res = 0;
  for (int i = 0; i < 4; ++i) {
    res = (res << 8) | bytes[i];
  }
  return res;
}

template <>
struct std::hash<State> {
  std::size_t operator()(const State &s) const {
    return hashCombine(s.hwDwellers << 8 | s.hwEmpty, hashCombine(asUint(s.homes), asUint(s.homesFree)));
  }
};

using Dists = std::unordered_map<State, Num>;

static State
ser(const Board &b, int n) {
  State s = {};
  for (int i = 0; i < HallwayPlaces.size(); ++i) {
    if (b[HallwayPlaces[i]] == -1) {
      s.hwEmpty |= (1 << i);
    } else {
      s.hwDwellers |= (b[HallwayPlaces[i]] << (i * 2));
    }
  }
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < n && b[i * 2 + 2, j + 1] == -1; ++j) {
      ++s.homesFree[i];
    }
    for (int j = 0; j < n - s.homesFree[i]; ++j) {
      s.homes[i] |= b[i * 2 + 2, j + s.homesFree[i] + 1] << (j * 2);
    }
  }
  return s;
}

static Board
deser(const State &s, int n) {
  Board b{11, n + 1, -1};
  for (int i = 0; i < HallwayPlaces.size(); ++i) {
    b[HallwayPlaces[i]] = (s.hwEmpty & (0x01 << i)) ? -1 : ((s.hwDwellers & (0x03 << (2 * i))) >> (i * 2));
  }
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < s.homesFree[i] && j < n; ++j) {
      b[i * 2 + 2, j + 1] = -1;
    }
    for (int j = 0; j < n - s.homesFree[i]; ++j) {
      b[i * 2 + 2, j + s.homesFree[i] + 1] = ((s.homes[i] & (0x03 << (j * 2))) >> (j * 2));
    }
  }
  return b;
}

struct Node {
  State s;
  Num dist;

  friend bool operator<(const Node &a, const Node &b) { return a.dist > b.dist; };
};

using Queue = std::priority_queue<Node>;

/* ------------------------------------------------------------------------ */

static bool
isFinal(const Board &b) {
  for (int y = 1; y < b.h(); ++y) {
    for (int i = 0; i < 4; ++i) {
      if (b[i * 2 + 2, y] != i) {
        return false;
      }
    }
  }
  return true;
}

static bool
isAllHome(const Board &b, Pos p) {
  for (; p.y < b.h(); ++p.y) {
    if (b[p] * 2 + 2 != p.x) {
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
  Num cost = Costs[b[p]] * (std::abs(to.x - p.x) + std::abs(to.y - p.y));
  auto dx = sgn(to.x - p.x);
  if (isHallway(p)) {
    while ((p.x += dx) != to.x) {
      if (b[p] != -1) {
        return -1;
      }
    }
    while (p.y != to.y) {
      ++p.y;
      if (b[p] != -1) {
        return -1;
      }
    }
  } else {
    while (--p.y != to.y) {
      if (b[p] != -1) {
        return -1;
      }
    }
    while (p.x != to.x) {
      p.x += dx;
      if (b[p] != -1) {
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
    auto ns = ser(nb, b.h() - 1);
    if (auto it = dists.find(ns); it == dists.end() || nd < it->second) {
      dists[ns] = nd;
      q.emplace(ns, nd);
      return true;
    }
  }
  return false;
}

static Num
solve(std::vector<string> lines) {
  // Dijkstra
  std::transform(lines.begin(), lines.end(), lines.begin(), [](auto line) { return line.substr(1, line.size() - 2); });
  Board start{lines.begin() + 1, lines.end() - 1, ' ', [](char c) { return ('A' <= c && c <= 'D') ? c - 'A' : -1; }};
  int n = start.h() - 1;
  Dists dists;
  auto q = Queue{};
  State s = ser(start, n);
  q.emplace(s, 0);
  dists[s] = 0;

  while (!q.empty()) {
    auto [s, dist] = q.top();
    q.pop();
    auto b = deser(s, n);
    if (dist != dists[s]) {
      continue;
    }
    if (isFinal(b)) {
      return dist;
    }
    for (auto hwp : HallwayPlaces) {
      char i = b[hwp];
      if (i == -1) {
        for (int i = 0; i < 4; ++i) {
          for (int y = 1; y < b.h(); ++y) {
            Pos hop{i * 2 + 2, y};
            if (b[hop] != -1) {
              if (!isAllHome(b, hop)) {
                tryMove(q, dists, dist, b, hop, hwp);
              }
              break;
            }
          }
        }
      } else {
        for (int y = b.h() - 1; y > 0; --y) {
          Pos hop{i * 2 + 2, y};
          if (b[hop] == -1) {
            tryMove(q, dists, dist, b, hwp, hop);
            break;
          } else if (b[hop] != i) {
            break;
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
