#include "../utils.h"
#include <unordered_map>

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

struct Edge {
  Pos pos;
  Num len;
};

/* ------------------------------------------------------------------------ */

using Edges = std::vector<Edge>;
using Graph = std::unordered_map<Pos, Edges>;

static void
junctions(const Board &b, const Pos &dst, Graph &g, Pos src, Pos pos, int dir, Num len) {
  if (pos == dst) {
    g[src].emplace_back(pos, len);
    return;
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
    return;
  }
  if (ways.size() > 1) {
    if (std::find_if(g[pos].begin(), g[pos].end(), [&](auto e) { return e.pos == src; }) != g[pos].end()) {
      return;
    }
    g[src].emplace_back(pos, len);
    g[pos].emplace_back(src, len);
    src = pos;
    len = 0;
  }
  for (auto d : ways) {
    Pos np = pos + DIRS[d];
    junctions(b, dst, g, src, np, d, len + 1);
  }
}

static Num
solveJunctions(const Graph &g, const Pos &dst, Path &path, Pos pos, Num len) {
  if (pos == dst) {
    return len;
  }
  Num res = 0;
  for (const auto &e : g.at(pos)) {
    if (std::find(path.begin(), path.end(), e.pos) != path.end()) {
      continue;
    }
    path.push_back(e.pos);
    res = std::max(res, solveJunctions(g, dst, path, e.pos, len + e.len));
    path.pop_back();
  }
  return res;
}

static Num
solve2(const Board &b, const Pos &dst, const Pos &src = {1, 0}, int dir = 2) {
  Graph g;
  junctions(b, dst, g, src, src, dir, 0);
  Path path;
  return solveJunctions(g, dst, path, src, 0);
}

/* ------------------------------------------------------------------------ */

int
main() {
  Board board = Board::read(std::cin, '#');

  Pos dst{board.w() - 2, board.h() - 1};
  println("1: {}", solve1(board, dst));
  Path path;
  println("2: {}", solve2(board, dst));
  return 0;
}
