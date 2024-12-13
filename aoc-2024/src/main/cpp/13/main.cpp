#include <iostream>
#include <string>
#include <sstream>

using Num = std::uint64_t;
using Double = long double;

static Num
solve(Num ax, Num ay, Num bx, Num by, Num gx, Num gy) {
  // ax * a + bx * b = gx
  // ay * a + by * b = gy

  Double m = static_cast<Double>(ay) / ax;
  Double b2 = by - m * bx;
  Double g2 = gy - m * gx;
  Num b = std::round(g2 / b2);
  Num a = (gx - bx * b) / ax;
  if (ax * a + bx * b == gx && ay * a + by * b == gy) {
    return 3 * a + b;
  }
  return 0;
}

static std::pair<Num, Num>
parseDir(std::string line) {
  // Button A: X+94, Y+34
  std::string skip;
  char c;
  int x, y;
  auto ss = std::stringstream{line};
  ss >> skip >> skip >> c >> c >> x >> c >> c >> c >> y;
  return {x, y};
}

static std::pair<Num, Num>
parseGoal(std::string line) {
  // Prize: X=8400, Y=5400
  std::string skip;
  char c;
  int x, y;
  auto ss = std::stringstream{line};
  ss >> skip >> c >> c >> x >> c >> c >> c >> y;
  return {x, y};
}

/* ------------------------------------------------------------------------ */

int
main() {
  std::string la, lb, lg, skip;
  Num res1 = 0, res2 = 0;
  static constexpr const auto MUL = 10000000000000UL;
  while (std::getline(std::cin, la) && std::getline(std::cin, lb) && std::getline(std::cin, lg)) {
    std::getline(std::cin, skip);
    auto [ax, ay] = parseDir(la);
    auto [bx, by] = parseDir(lb);
    auto [gx, gy] = parseGoal(lg);

    res1 += solve(ax, ay, bx, by, gx, gy);
    res2 += solve(ax, ay, bx, by, MUL + gx, MUL + gy);
  }

  std::cout << "1: " << res1 << "\n";
  std::cout << "2: " << res2 << "\n";
  return 0;
}
