#include "../utils.h"
#include <iostream>
#include <queue>

/* ------------------------------------------------------------------------ */

using Board = Grid<>;
using Num = int;

struct Node {
  Pos pos;
  Num cost;

  friend bool operator<(const Node &a, const Node &b) { return a.cost > b.cost; };
};

using Queue = std::priority_queue<Node>;

/* ------------------------------------------------------------------------ */

static std::pair<Num, Board>
solve(const Board &board, Pos start, Pos end) {
  auto path = Board(board);
  auto costs = Grid<Num>(board.w(), board.h(), std::numeric_limits<Num>::max(), std::numeric_limits<Num>::max());
  auto prevs = Grid<Pos>(board.w(), board.h(), {-1, -1});
  auto queue = Queue();
  queue.emplace(start, 0);
  costs[start] = 0;
  while (!queue.empty()) {
    Node n = queue.top();
    queue.pop();
    if (n.pos == end) {
      for (Pos p = n.pos; p != Pos{-1, -1}; p = prevs[p]) {
        path[p] = 'O';
      }
      return {n.cost, path};
    }
    if (n.cost != costs[n.pos]) {
      continue;
    }
    for (Dir d : DIRS) {
      auto nn = Node{n.pos + d, n.cost + 1};
      auto cc = costs[nn.pos];
      if (board[nn.pos] == '#' || nn.cost >= cc) {
        continue;
      }
      costs[nn.pos] = nn.cost;
      prevs[nn.pos] = n.pos;
      queue.push(nn);
    }
  }
  return {-1, path};
}

/* ------------------------------------------------------------------------ */

int
main() {
  const Pos startPos = {0, 0};
  const Pos endPos = {70, 70};
  const int count = 1024;

  std::vector<Pos> blocks;
  Pos p;
  char c;
  while (std::cin >> p.x >> c >> p.y) {
    blocks.push_back(p);
  }
  Board board = Board(endPos.x + 1, endPos.y + 1, '.', '#');
  for (auto it = blocks.begin(), end = blocks.begin() + count; it != end; ++it) {
    board[*it] = '#';
  }

  auto [res1, path] = solve(board, startPos, endPos);
  std::cout << "1: " << res1 << "\n";

  for (auto it = blocks.begin() + count, end = blocks.end(); it != end; ++it) {
    board[*it] = '#';
    if (path[*it] != 'O') {
      continue;
    }
    auto [cost, pp] = solve(board, startPos, endPos);
    if (cost < 0) {
      std::cout << it->x << "," << it->y << "\n";
      return 0;
    }
    std::swap(path, pp);
  }

  return 0;
}
