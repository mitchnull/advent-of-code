#include "../utils.h"

using Num = int64_t;

static std::pair<Pos, Dir>
move1(Pos p, Dir d, char c, int n) {
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
    case 'N': p.y -= n; break;
    case 'E': p.x += n; break;
    case 'S': p.y += n; break;
    case 'W': p.x -= n; break;
  }
  return {p, d};
}

static std::pair<Pos, Dir>
move2(Pos p, Dir wp, char c, int n) {
  switch (c) {
    case 'L':
      if (n == 90) {
        wp = {wp.dy, -wp.dx};
      } else if (n == 180) {
        wp = -wp;
      } else if (n == 270) {
        wp = {-wp.dy, wp.dx};
      }
      break;
    case 'R':
      if (n == 270) {
        wp = {wp.dy, -wp.dx};
      } else if (n == 180) {
        wp = -wp;
      } else if (n == 90) {
        wp = {-wp.dy, wp.dx};
      }
      break;
    case 'F': p += wp * n; break;
    case 'N': wp.dy -= n; break;
    case 'E': wp.dx += n; break;
    case 'S': wp.dy += n; break;
    case 'W': wp.dx -= n; break;
  }
  return {p, wp};
}

/* ------------------------------------------------------------------------ */

int
main() {
  Pos p1 = {}, p2 = {};
  Dir d1 = {1, 0}, wp = {10, -1};
  char c;
  int n;
  while (std::cin >> c >> n) {
    std::tie(p1, d1) = move1(p1, d1, c, n);
    std::tie(p2, wp) = move2(p2, wp, c, n);
  }

  println("1: {}", std::abs(p1.x) + std::abs(p1.y));
  println("1: {}", std::abs(p2.x) + std::abs(p2.y));

  return 0;
}
