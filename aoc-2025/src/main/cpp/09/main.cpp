#include "../utils.h"
#include <deque>

using Num = int64_t;
using V = std::vector<Pos>;
using Board = Grid<>;

static void
drawLine(Board& board, Pos a, Pos b) {
  if (a.x == b.x) {
    for (int y = std::min(a.y, b.y); y <= std::max(a.y, b.y); ++y) {
      board[a.x, y] = '#';
    }
  } else {
    for (int x = std::min(a.x, b.x); x <= std::max(a.x, b.x); ++x) {
      board[x, a.y] = '#';
    }
  }
}

static void
fill(Board& board, Pos p) {
  std::deque<Pos> q{p};
  while (!q.empty()) {
    p = q.front();
    q.pop_front();
    if (board[p] != '.') {
      continue;
    }
    board[p] = '@';
    for (Dir d : DIRS) {
      q.push_back(p + d);
    }
  }
}

static bool
check(const Board& board, Pos a, Pos b) {
  Pos tl{std::min(a.x, b.x), std::min(a.y, b.y)}, br{std::max(a.x, b.x), std::max(a.y, b.y)};
  for (int y = tl.y; y <= br.y; ++y) {
    for (int x = tl.x; x <= br.x; ++x) {
      if (board[x, y] == '@') {
        return false;
      }
    }
  }
  return true;
}

static Num
solve2(const V& v) {
  Num res2{};
  const int N = v.size();
  Pos br{};
  for (const auto& p: v) {
    br = { std::max(br.x, p.x), std::max(br.y, p.y) };
  }
  Board board(br.x + 2, br.y + 2, '.', '@');
  for (int i = 1; i < N; ++i) {
    drawLine(board, v[i - 1], v[i]);
  }
  drawLine(board, v[N - 1], v[0]);
  fill(board, Pos{});
  for (int i = 0; i < N; ++i) {
    for (int j = i + 1; j < N; ++j) {
      Num a = (std::abs(v[j].x - v[i].x) + Num{1}) * (std::abs(v[j].y - v[i].y) + Num{1});
      if (a > res2 && check(board, v[i], v[j])) {
        res2 = a;
      }
    }
  }
  return res2;
}


/* ------------------------------------------------------------------------ */

int
main() {
  Num res1{}, res2{};
  char c;
  Pos p;

  V v;
  while (std::cin >> p.x >> c >> p.y) {
    v.push_back(p);
  }
  const int N = v.size();
  for (int i = 0; i < N; ++i) {
    for (int j = i + 1; j < N; ++j) {
      Num a = (std::abs(v[j].x - v[i].x) + Num{1}) * (std::abs(v[j].y - v[i].y) + Num{1});
      if (a > res1) {
        res1 = a;
      }
    }
  }

  res2 = solve2(v);
  println("1: {}", res1);
  println("2: {}", res2);

  return 0;
}
