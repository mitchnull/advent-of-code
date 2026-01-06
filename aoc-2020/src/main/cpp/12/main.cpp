#include "../utils.h"

using Num = int64_t;

static std::pair<Pos, Dir>
move1(Pos p, Dir d, char c, int n) {
  switch (c) {
    case 'L':
      if (n == 90) {
        d = { d.dy, -d.dx };
      } else if (n == 180) {
        d = -d;
      } else if (n == 270) {
        d = { -d.dy, d.dx };
      }
      break;
    case 'R':
      if (n == 270) {
        d = { d.dy, -d.dx };
      } else if (n == 180) {
        d = -d;
      } else if (n == 90) {
        d = { -d.dy, d.dx };
      }
      break;
    case 'F': p += d * n; break;
    case 'N': p.y -= n; break;
    case 'E': p.x += n; break;
    case 'S': p.y += n; break;
    case 'W': p.x -= n; break;
  }
  return {p, d};
}

/* ------------------------------------------------------------------------ */

int
main() {
  Pos p1 = {};
  Dir d1 = {1, 0};
  char c;
  int n;
  while (std::cin >> c >> n) {
    std::tie(p1, d1) = move1(p1, d1, c, n);
  }

  println("1: {}", std::abs(p1.x) + std::abs(p1.y));

  return 0;
}
