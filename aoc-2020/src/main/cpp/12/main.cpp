#include "../utils.h"

using Num = int64_t;

static std::pair<Pos, Dir>
move(Pos p, Dir d, char c, int n, bool wp) {
  switch (c) {
    case 'L':
      if (n == 90) {
        d = {d.dy, -d.dx};
      } else if (n == 180) {
        d = -d;
      } else if (n == 270) {
        d = {-d.dy, d.dx};
      }
      break;
    case 'R':
      if (n == 270) {
        d = {d.dy, -d.dx};
      } else if (n == 180) {
        d = -d;
      } else if (n == 90) {
        d = {-d.dy, d.dx};
      }
      break;
    case 'F': p += d * n; break;
    case 'N': (wp ? d.dy : p.y) -= n; break;
    case 'E': (wp ? d.dx : p.x) += n; break;
    case 'S': (wp ? d.dy : p.y) += n; break;
    case 'W': (wp ? d.dx : p.x) -= n; break;
  }
  return {p, d};
}

/* ------------------------------------------------------------------------ */

int
main() {
  Pos p1 = {}, p2 = {};
  Dir d1 = {1, 0}, wp = {10, -1};
  char c;
  int n;
  while (std::cin >> c >> n) {
    std::tie(p1, d1) = move(p1, d1, c, n, false);
    std::tie(p2, wp) = move(p2, wp, c, n, true);
  }

  println("1: {}", std::abs(p1.x) + std::abs(p1.y));
  println("1: {}", std::abs(p2.x) + std::abs(p2.y));

  return 0;
}
