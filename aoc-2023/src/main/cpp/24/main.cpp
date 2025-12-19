#include "../gmpxx.h"
#include "../utils.h"
#include <z3++.h>

using Num = mpz_class;

/* ------------------------------------------------------------------------ */

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

struct HailStone {
  Num x, y, z, dx, dy, dz;
};

using HailStones = std::vector<HailStone>;

Num
solve2(const HailStones &hss) {
  z3::context ctx;
  z3::solver s{ctx};

  z3::expr x = ctx.int_const("x");
  z3::expr y = ctx.int_const("y");
  z3::expr z = ctx.int_const("z");
  z3::expr dx = ctx.int_const("dx");
  z3::expr dy = ctx.int_const("dy");
  z3::expr dz = ctx.int_const("dz");
  for (int i = 0; i < hss.size(); ++i) {
    z3::expr ti = ctx.int_const(std::format("t{}", i).c_str());
    s.add(ti >= 0);
    z3::expr xi = ctx.int_val(hss[i].x.get_str().c_str());
    z3::expr yi = ctx.int_val(hss[i].y.get_str().c_str());
    z3::expr zi = ctx.int_val(hss[i].z.get_str().c_str());
    z3::expr dxi = ctx.int_val(hss[i].dx.get_str().c_str());
    z3::expr dyi = ctx.int_val(hss[i].dy.get_str().c_str());
    z3::expr dzi = ctx.int_val(hss[i].dz.get_str().c_str());
    s.add((x + dx * ti) == (xi + dxi * ti));
    s.add((y + dy * ti) == (yi + dyi * ti));
    s.add((z + dz * ti) == (zi + dzi * ti));
  }
  s.check();
  return Num(s.get_model().eval(x + y + z).to_string().c_str());
}

/* ------------------------------------------------------------------------ */

int
main() {
  char ch;
  HailStone hs;
  Lines2d lines2d;
  HailStones hss;

  while (std::cin >> hs.x >> ch >> hs.y >> ch >> hs.z >> ch >> hs.dx >> ch >> hs.dy >> ch >> hs.dz) {
    lines2d.emplace_back(hs.x, hs.y, hs.dx, hs.dy);
    hss.push_back(hs);
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
  println("2: {}", solve2(hss));

  return 0;
}
