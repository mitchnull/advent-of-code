#include "../utils.h"
#include <unordered_map>
#include <deque>

using Num = int;
using Board = Grid<>;

struct Node {
  Pos pos;
  int dir;
  Num len;
};

static const auto DIR_SYMS = "^>v<";

static Num
solve1(const Board& b) {
  Num res = 0;
  Pos dst = {b.w() - 2, b.h() - 1};
  std::deque<Node> q{Node{Pos{1, 0}, 2, 0}};
  while (!q.empty()) {
    Node n = q.front();
    q.pop_front();
    if (n.pos == dst) {
      res = std::max(res, n.len);
    }
    for (int d = 0; d < 4; ++d) {
      if (d != n.dir && ((d + n.dir) % 2 == 0)) {
        continue;
      }
      Pos np = n.pos + DIRS[d];
      if (b[np] == '.' || b[np] == DIR_SYMS[d]) {
        q.emplace_back(np, d, n.len + 1);
      }
    }
  }
  return res;
}

/* ------------------------------------------------------------------------ */

int
main() {
  Board board = Board::read(std::cin, '#');

  println("1: {}", solve1(board));
  return 0;
}
