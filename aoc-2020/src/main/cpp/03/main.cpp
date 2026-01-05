#include "../utils.h"
#include <numeric>

using Num = int64_t;
using Board = Grid<>;

static const std::vector<Dir> Slopes = {
    {1, 1},
    {3, 1},
    {5, 1},
    {7, 1},
    {1, 2},
};

static Num
solve(const Board &b, Dir d) {
  Num res = 0;
  for (Pos p = {}; p.y < b.h(); p = {(p.x + d.dx) % b.w(), p.y + d.dy}) {
    res += b[p] == '#';
  }
  return res;
};

/* ------------------------------------------------------------------------ */

int
main() {
  auto b = Board::read();

  println("1: {}", solve(b, {3, 1}));
  println("2: {}", std::transform_reduce(begin(Slopes), end(Slopes), Num{1}, std::multiplies(), [&](auto d) {
    return solve(b, d);
  }));

  return 0;
}
