#include <iostream>
#include <queue>
#include <unordered_map>
#include "../utils.h"

/* ------------------------------------------------------------------------ */

using Board = Grid<>;
using Num = int;

struct Cheat {
  Pos start, end;

  friend bool operator<=>(const Cheat&a, const Cheat&b)  = default;
};

template <>
struct std::hash<Cheat> {
  std::size_t operator()(const Cheat& c) const {
    return std::hash<Pos>()(c.start) * 11 + std::hash<Pos>()(c.end);
  }
};


struct Node {
  Pos pos;
  Num cost;

  friend bool operator<(const Node&a, const Node&b) { return a.cost > b.cost; };
};

using Queue = std::priority_queue<Node>;

/* ------------------------------------------------------------------------ */

static std::pair<Num, Pos>
solve(const Board& board, Pos start, Pos end, Pos cheatStart) {
  auto costs = Grid<Num>(board.w(), board.h(), std::numeric_limits<Num>::max(), std::numeric_limits<Num>::max());
  auto prevs = Grid<Pos>(board.w(), board.h(), {-1, -1});
  auto queue = Queue();
  queue.emplace(start, 0);
  costs[start] = 0;
  while (!queue.empty()) {
    Node n = queue.top();
    queue.pop();
    if (n.pos == end) {
      for (Pos p = n.pos, pp = {}; p != Pos{-1, -1}; pp = p, p = prevs[p]) {
        if (p == cheatStart) {
          return {n.cost, pp};
        }
      }
      return {n.cost, {}};
    }
    if (n.cost != costs[n.pos]) {
      continue;
    }
    for (Dir d : DIRS) {
      auto nn = Node{n.pos + d, n.cost + 1};
      auto cc = costs[nn.pos];
      if ((board[nn.pos] == '#' && nn.pos != cheatStart) || nn.cost >= cc) {
        continue;
      }
      costs[nn.pos] = nn.cost;
      prevs[nn.pos] = n.pos;
      queue.push(nn);
    }
  }
  return {-1, {}};
}

/* ------------------------------------------------------------------------ */

int
main() {
  std::vector<std::string> lines;
  std::string line;
  while (std::getline(std::cin, line)) {
    lines.push_back(line); 
  }
  Board board = Board(lines, '#');

  Pos startPos = (board.iter() | views::filter([](auto i) { return i.v == 'S'; }) | views::transform([](auto i) { return Pos{i.x, i.y}; })).front();
  Pos endPos = (board.iter() | views::filter([](auto i) { return i.v == 'E'; }) | views::transform([](auto i) { return Pos{i.x, i.y}; })).front();

  auto [origCost, ig] = solve(board, startPos, endPos, {});
  std::unordered_map<Cheat, Num> cheats;
  for (auto [x, y, v] : board.iter()) {
    Pos cheatStart = {x, y};
    if (v == '#') {
      auto [cost, cheatEnd] = solve(board, startPos, endPos, cheatStart);
      if (origCost - cost >= 100 && cheatEnd != Pos{}) {
        Cheat cheat = {cheatStart, cheatEnd};
        cheats[cheat] = std::max(cheats[cheat], cost);
      }
    }
  }
  auto res1 = cheats.size();
  std::cout << "1: " << res1 << "\n";

  return 0;
}
