#include <iostream>
#include <queue>
#include "../utils.h"

/* ------------------------------------------------------------------------ */

using Board = Grid<>;
using Num = int;
using Path = std::vector<Pos>;

struct Node {
  Pos pos;
  Num cost;

  friend bool operator<(const Node&a, const Node&b) { return a.cost > b.cost; };
};

using Queue = std::priority_queue<Node>;

/* ------------------------------------------------------------------------ */

static Path
findPath(const Board& board, Pos start, Pos end) {
  auto path = Path{};
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
        path.push_back(p);
      }
      return path;
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
  return path;
}

static std::pair<Num, Num>
solve(const Path& path, int maxCheat1, int maxCheat2, int minGain) {
  Num res1 = 0, res2 = 0;
  for (auto i = path.begin(), end = path.end(); i != end; ++i) {
    for (auto j = i + minGain; j < end; ++j) {
      auto d = std::abs(i->x - j->x) + std::abs(i->y - j->y);
      if (((j - i) - d) >= minGain) {
        res1 += d <= maxCheat1;
        res2 += d <= maxCheat2;
      }
    }
  }
  return {res1, res2};
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

  auto path = findPath(board, startPos, endPos);
  auto [res1, res2] = solve(path, 2, 20, 100);

  std::cout << "1: " << res1 << "\n";
  std::cout << "2: " << res2 << "\n";

  return 0;
}
