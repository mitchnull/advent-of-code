#include "../utils.h"
#include <regex>

// y = (2 * y0 - t + 1) * t / 2;
// (2 * y) / t = 2 * y0 - t + 1
// ((2 * y) / t + t - 1) / 2 = y0

static int
height(int y) {
  int tm = (2 * y + 1) / 2;
  return (2 * y - tm + 1) * tm / 2;
}

static bool
check(const Pos &tl, const Pos &br, Dir d) {
  for (Pos p = {0, 0}; p.y >= br.y; p += d, d = { std::max(0, d.dx - 1), d.dy - 1 }) {
    if (tl.x <= p.x && p.x <= br.x && tl.y >= p.y && p.y >= br.y) {
      return true;
    }
  }
  return false;
}

static int
solve1(Pos tl, Pos br) {
  int res = 0;
  if ((tl.x < 0 && br.x < 0) || (tl.x < 0 && -tl.x < br.x)) {
    std::tie(tl.x, br.x) = std::make_pair(-br.x, -tl.x);
  }
  int mx = std::max(std::abs(tl.x), std::abs(br.x));
  int my = std::max(std::abs(tl.y), std::abs(br.y));
  for (int y = 0; y < my; ++y) {
    for (int x = 0; x < mx; ++x) {
      if (check(tl, br, {x, y})) {
        res = std::max(res, height(y));
      }
    }
  }
  return res;
}

/* ------------------------------------------------------------------------ */

int
main() {
  string line;
  std::getline(std::cin, line);

  std::regex re("([0-9-]+)");
  auto it = std::sregex_iterator(line.begin(), line.end(), re);
  auto xmin = std::stoi(it++->str());
  auto xmax = std::stoi(it++->str());
  auto ymax = std::stoi(it++->str());
  auto ymin = std::stoi(it++->str());

  println("1: {}", solve1({xmin, ymin}, {xmax, ymax}));

  return 0;
}
