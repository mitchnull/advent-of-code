#include "../utils.h"
#include <deque>

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
  Path fullPath;
};

static const auto DIR_SYMS = "^>v<";

static Num
solve1(const Board &b) {
  Num res = 0;
  Pos dst = {b.w() - 2, b.h() - 1};
  std::deque<Node> q{Node{Pos{1, 0}, 2, 0}};
  while (!q.empty()) {
    auto n = q.front();
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

static Num
solve2(const Board &b) {
  Num res = 0;
  Pos dst = {b.w() - 2, b.h() - 1};
  std::deque<Node2> q{{{Pos{1, 0}, 2, 0}, {}}};
  println("{}", b); // @@@
  while (!q.empty()) {
    auto n = q.front();
    q.pop_front();
    println("@@@ n: {}, {}, {}", n.pos, DIR_SYMS[n.dir], n.len);
    if (n.pos == dst) {
      res = std::max(res, n.len);
    }
    std::vector<int> ways;
    for (int d = 0; d < 4; ++d) {
      if (d != n.dir && ((d + n.dir) % 2 == 0)) {
        continue;
      }
      if (b[n.pos + DIRS[d]] != '#') {
        ways.push_back(d);
      }
    }
    if (ways.empty()) {
      continue;
    }
    Path path = n.path;
    if (ways.size() > 1) {
      if (std::find(path.begin(), path.end(), n.pos) != path.end()) {
        println("@@@ skipping junction at {}, path: {}", n.pos, n.path);
        auto bb = b;
        auto c = '0';
        for (auto p : n.path) {
          bb[p] = c;
          if (c == '9') {
            c = 'A';
          } else if (c == 'Z') {
            c = 'a';
          } else {
            ++c;
          }
        }
        println("{}", bb);
        continue;
      }
      println("@@@ junction at {}, path: {}", n.pos, n.path);
      path.push_back(n.pos);
    }
    Path fullPath = n.fullPath;
    if (std::find(fullPath.begin(), fullPath.end(), n.pos) != fullPath.end()) {
      println("@@@ loop at {}, {}", n.pos, n.fullPath);
      auto bb = b;
      for (auto p : n.fullPath) {
        bb[p] = 'o';
      }
      auto c = '0';
      for (auto p : n.path) {
        bb[p] = c;
        if (c == '9') {
          c = 'a';
        } else if (c == 'z') {
          c = 'a';
        } else {
          ++c;
        }
      }
      println("{}", bb);
      abort();
    }
    fullPath.push_back(n.pos);
    println("@@@ ways: {}", ways);
    {
      auto bb = b;
      for (auto p : n.fullPath) {
        bb[p] = 'o';
      }
      auto c = '0';
      for (auto p : n.path) {
        bb[p] = c;
        if (c == '9') {
          c = 'a';
        } else if (c == 'z') {
          c = 'a';
        } else {
          ++c;
        }
      }
      println("{}", bb);
    }
    for (auto d : ways) {
      Pos np = n.pos + DIRS[d];
      q.push_back({{np, d, n.len + 1}, path, fullPath});
    }
  }
  return res;
}

/* ------------------------------------------------------------------------ */

int
main() {
  Board board = Board::read(std::cin, '#');

  println("1: {}", solve1(board));
  println("2: {}", solve2(board));
  return 0;
}
