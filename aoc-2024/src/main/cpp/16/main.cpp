#include "../utils.h"
#include <iostream>
#include <queue>
#include <ranges>
#include <string>
#include <unordered_map>
#include <unordered_set>

/* ------------------------------------------------------------------------ */

struct PosDir {
  Pos pos;
  Dir dir;

  auto friend operator<=>(const PosDir &a, const PosDir &b) = default;
};

template <>
struct std::hash<PosDir> {
  std::size_t operator()(const PosDir &pd) const { return hashCombine(std::hash<Pos>{}(pd.pos), pd.dir); }
};

using Board = Grid<>;
using Num = int;

struct Node {
  PosDir v;
  Num cost;

  friend bool operator<(const Node &a, const Node &b) { return a.cost > b.cost; };
};

using Queue = std::priority_queue<Node>;

/* ------------------------------------------------------------------------ */

static void
findBestPlaces(const std::unordered_map<PosDir, std::unordered_set<PosDir>> &prevs,
    PosDir pd,
    std::unordered_set<PosDir> &bestPlaces) {
  if (bestPlaces.contains(pd)) {
    return;
  }
  bestPlaces.insert(pd);

  auto pp = prevs.find(pd);
  if (pp == prevs.end()) {
    return;
  }
  for (auto pd : pp->second) {
    findBestPlaces(prevs, pd, bestPlaces);
  }
}

static Num
findCost(const auto &costs, const PosDir &v) {
  auto cp = costs.find(v);
  return cp != costs.end() ? cp->second : std::numeric_limits<Num>::max();
}

static std::pair<Num, Num>
solve(const Board &board, PosDir start, Pos end) {
  auto costs = std::unordered_map<PosDir, Num>();
  auto prevs = std::unordered_map<PosDir, std::unordered_set<PosDir>>();
  auto queue = Queue();
  queue.emplace(start, 0);
  while (!queue.empty()) {
    Node n = queue.top();
    queue.pop();
    if (n.cost != costs[n.v]) {
      continue;
    }
    for (Node nn : {Node{PosDir{n.v.pos + n.v.dir, n.v.dir}, n.cost + 1},
             Node{PosDir{n.v.pos, Dir{n.v.dir.dy, n.v.dir.dx}}, n.cost + 1000},
             Node{PosDir{n.v.pos, Dir{-n.v.dir.dy, -n.v.dir.dx}}, n.cost + 1000}}) {
      auto cc = findCost(costs, nn.v);
      if (board[nn.v.pos] == '#' || nn.cost > cc) {
        continue;
      }
      if (nn.cost < cc) {
        costs[nn.v] = nn.cost;
        prevs[nn.v] = {n.v};
      } else {
        prevs[nn.v].insert(n.v);
      }
      queue.push(nn);
    }
  }

  Num minCost = std::numeric_limits<Num>::max();
  auto bestPlaces = std::unordered_set<PosDir>{};
  for (auto dir : DIRS) {
    auto endDir = PosDir{end, dir};
    auto cp = costs.find(endDir);
    if (cp != costs.end() && cp->second < minCost) {
      minCost = cp->second;
      bestPlaces.clear();
    }
    if (cp->second == minCost) {
      findBestPlaces(prevs, endDir, bestPlaces);
    }
  }
  auto numBestPlaces =
      (bestPlaces | views::transform([](auto pd) { return pd.pos; }) | std::ranges::to<std::unordered_set<Pos>>())
          .size();
  return {minCost, numBestPlaces};
}

/* ------------------------------------------------------------------------ */

int
main() {
  std::vector<std::string> lines;
  std::string line;
  while (std::getline(std::cin, line)) {
    lines.push_back(line);
  }
  Board board = Board(lines);
  Pos start = (board.iter() | views::filter([](auto i) { return i.v == 'S'; }) | views::transform([](auto i) {
    return Pos{i.x, i.y};
  })).front();
  Pos end = (board.iter() | views::filter([](auto i) { return i.v == 'E'; }) | views::transform([](auto i) {
    return Pos{i.x, i.y};
  })).front();

  auto [res1, res2] = solve(board, {start, {1, 0}}, end);

  std::cout << "1: " << res1 << "\n";
  std::cout << "2: " << res2 << "\n";

  return 0;
}
