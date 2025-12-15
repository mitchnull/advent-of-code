#include "../utils.h"
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>

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

using Board = Grid<int>;
using Num = int;

struct Node {
  PosDir v;
  Num cost;

  friend bool operator<(const Node &a, const Node &b) { return a.cost > b.cost; };
};

using Queue = std::priority_queue<Node>;

/* ------------------------------------------------------------------------ */

static Num
findCost(const auto &costs, const PosDir &v) {
  auto cp = costs.find(v);
  return cp != costs.end() ? cp->second : std::numeric_limits<Num>::max();
}

static Num
solve(const Board &board, Pos start, Pos end, int minSteps, int maxSteps) {
  auto costs = std::unordered_map<PosDir, Num>();
  auto prevs = std::unordered_map<PosDir, PosDir>();
  auto queue = Queue();
  queue.emplace(PosDir{start, Dir{1, 0}}, 0);
  queue.emplace(PosDir{start, Dir{0, 1}}, 0);
  while (!queue.empty()) {
    auto [pd, cost] = queue.top();
    queue.pop();
    if (cost != costs[pd]) {
      continue;
    }
    if (pd.pos == end) {
      return costs[pd];
    }
    for (Dir dir : {Dir{pd.dir.dy, -pd.dir.dx}, Dir{-pd.dir.dy, pd.dir.dx}}) {
      Num nc = cost;
      auto np = pd.pos;
      for (int i = 1; i <= maxSteps; ++i) {
        np += dir;
        auto npc = board[np];
        if (!npc) {
          break;
        }
        nc += npc;
        auto npd = PosDir{np, dir};
        if (i >= minSteps && nc < findCost(costs, npd)) {
          costs[npd] = nc;
          prevs[npd] = pd;
          queue.emplace(npd, nc);
        }
      }
    }
  }
  return 0;
}

/* ------------------------------------------------------------------------ */

int
main() {
  std::vector<std::string> lines;
  std::string line;
  while (std::getline(std::cin, line)) {
    lines.push_back(line);
  }
  Board board = Board(lines, 0, [](auto c) { return c - '0'; });

  auto res1 = solve(board, {0, 0}, {board.w() - 1, board.h() - 1}, 1, 3);
  auto res2 = solve(board, {0, 0}, {board.w() - 1, board.h() - 1}, 4, 10);

  std::cout << res1 << "\n";
  std::cout << res2 << "\n";
}
