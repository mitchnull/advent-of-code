#include "../utils.h"

using Board = Grid<>;

static int
move(Board &b, char cuc, Dir d) {
  std::vector<Pos> moving;
  for (int y = 0; y < b.h(); ++y) {
    for (int x = 0; x < b.w(); ++x) {
      if (b[x, y] == cuc) {
        if (b[(x + d.dx) % b.w(), (y + d.dy) % b.h()] == '.') {
          moving.emplace_back(x, y);
        }
      }
    }
  }
  for (const auto &p : moving) {
    b[p] = '.';
    b[(p.x + d.dx) % b.w(), (p.y + d.dy) % b.h()] = cuc;
  }
  return moving.size();
}

static int
solve(Board b) {
  for (int moves = 1;; ++moves) {
    int east = move(b, '>', Dir{1, 0});
    if (east + move(b, 'v', Dir{0, 1}) == 0) {
      return moves;
    }
  }
}

/* ------------------------------------------------------------------------ */

int
main() {
  Board b = Board::read();
  println("1: {}", solve(b));
  return 0;
}
