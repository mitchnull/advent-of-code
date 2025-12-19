#include "../utils.h"

using Num = int;
using Board = Grid<>;
using Path = std::vector<Pos>;

struct Node {
  Pos pos;
  int dir;
  Num len;
};

struct Node2 : public Node {
  Path path;
};

static const auto DIR_SYMS = "^>v<";

static Num
solve1(const Board &b, const Pos &dst, Pos pos = {1, 0}, int dir = 2, Num len = 0) {
  if (pos == dst) {
    return len;
  }
  Num res = 0;
  for (int d = 0; d < 4; ++d) {
    if (d != dir && ((d + dir) % 2 == 0)) {
      continue;
    }
    Pos np = pos + DIRS[d];
    if (b[np] == '.' || b[np] == DIR_SYMS[d]) {
      res = std::max(res, solve1(b, dst, np, d, len + 1));
    }
  }
  return res;
}

static Num
solve2(const Board &b, const Pos &dst, Path &path, Pos pos = {1, 0}, int dir = 2, Num len = 0) {
  if (pos == dst) {
    return len;
  }
  std::vector<int> ways;
  for (int d = 0; d < 4; ++d) {
    if (d != dir && ((d + dir) % 2 == 0)) {
      continue;
    }
    if (b[pos + DIRS[d]] != '#') {
      ways.push_back(d);
    }
  }
  if (ways.empty()) {
    return 0;
  }
  if (ways.size() > 1) {
    if (std::find(path.begin(), path.end(), pos) != path.end()) {
      return 0;
    }
    path.push_back(pos);
  }
  Num res = 0;
  for (auto d : ways) {
    Pos np = pos + DIRS[d];
    res = std::max(res, solve2(b, dst, path, np, d, len + 1));
  }
  if (ways.size() > 1) {
    path.pop_back();
  }
  return res;
}

/* ------------------------------------------------------------------------ */

int
main() {
  Board board = Board::read(std::cin, '#');

  Pos dst{board.w() - 2, board.h() - 1};
  println("1: {}", solve1(board, dst));
  Path path;
  println("2: {}", solve2(board, dst, path));
  return 0;
}
