#include "../utils.h"
#include <regex>

static int
height(int y) {
  int tm = std::max(0, (2 * y + 1) / 2);
  return (2 * y - tm + 1) * tm / 2;
}

static bool
check(const Pos &tl, const Pos &br, Dir d) {
  for (Pos p = {0, 0}; p.y >= br.y; p += d, d = {std::max(0, d.dx - 1), d.dy - 1}) {
    if (tl.x <= p.x && p.x <= br.x && tl.y >= p.y && p.y >= br.y) {
      return true;
    }
  }
  return false;
}

static std::pair<int, int>
solve(Pos tl, Pos br) {
  int res1 = 0, res2 = 0;
  if ((tl.x < 0 && br.x < 0) || (tl.x < 0 && -tl.x < br.x)) {
    std::tie(tl.x, br.x) = std::make_pair(-br.x, -tl.x);
  }
  int mx = std::max(std::abs(tl.x), std::abs(br.x));
  int my = std::max(std::abs(tl.y), std::abs(br.y));
  for (int y = -my; y <= my; ++y) {
    for (int x = -mx; x <= mx; ++x) {
      if (check(tl, br, {x, y})) {
        res1 = std::max(res1, height(y));
        ++res2;
      }
    }
  }
  return {res1, res2};
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

  auto [res1, res2] = solve({xmin, ymin}, {xmax, ymax});
  println("1: {}", res1);
  println("2: {}", res2);

  return 0;
}
