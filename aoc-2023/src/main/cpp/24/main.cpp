#include "../gmpxx.h"
#include "../utils.h"

using Num = mpz_class;

struct Pos2d {
  Num x, y;
};

struct Line2d {
  Num a, b, c, x, y, dx, dy;

  Line2d(Num a, Num b, Num c) : a(a), b(b), c(c), x{}, y{}, dx{}, dy{} {}
  Line2d(Num x, Num y, Num dx, Num dy) : a(dy), b(-dx), c(dx * y - dy * x), x(x), y(y), dx(dx), dy(dy) {}
  bool isInFuture(const Pos2d &p) const { return sgn(p.x - x) == sgn(dx) && sgn(p.y - y) == sgn(dy); }
};

using Lines2d = std::vector<Line2d>;

static Line2d
intersect(const Line2d &e, const Line2d &f) {
  return {e.b * f.c - f.b * e.c, f.a * e.c - e.a * f.c, e.a * f.b - f.a * e.b};
}

static bool
intersectInBounds(const Line2d &e, const Line2d &f, const Num &bmin, const Num &bmax) {
  auto isect = intersect(e, f);
  if (isect.c == 0) {
    return false;
  }
  Pos2d ip{isect.a / isect.c, isect.b / isect.c};
  return bmin <= ip.x && ip.x <= bmax && bmin <= ip.y && ip.y <= bmax && e.isInFuture(ip) && f.isInFuture(ip);
}

/* ------------------------------------------------------------------------ */

int
main() {
  char ch;
  Num x, y, z, dx, dy, dz;
  Lines2d lines2d;
  while (std::cin >> x >> ch >> y >> ch >> z >> ch >> dx >> ch >> dy >> ch >> dz) {
    lines2d.emplace_back(x, y, dx, dy);
  }

  Num res1 = 0;
  Num bmin = lines2d.size() < 100 ? 7 : 200000000000000L;
  Num bmax = lines2d.size() < 100 ? 27 : 400000000000000L;
  for (int i = 0; i < lines2d.size(); ++i) {
    for (int j = i + 1; j < lines2d.size(); ++j) {
      res1 += intersectInBounds(lines2d[i], lines2d[j], bmin, bmax);
    }
  }

  println("1: {}", res1);

  return 0;
}
