#include "../utils.h"

using Num = int64_t;
using V = std::vector<Pos>;
struct Line { Pos a, b; };
using Lines = std::vector<Line>;

static bool
check(const Lines& lines, Pos a, Pos b) {
  Pos tl{std::min(a.x, b.x), std::min(a.y, b.y)}, br{std::max(a.x, b.x), std::max(a.y, b.y)};
  for (const auto& line: lines) {
    if ((tl.y < line.a.y || tl.y < line.b.y) && (line.a.y < br.y || line.b.y < br.y)
        && (tl.x < line.a.x || tl.x < line.b.x) && (line.a.x < br.x || line.b.x < br.x)) {
      return false;
    }
  }
  return true;
}

/* ------------------------------------------------------------------------ */

int
main() {
  Num res1{}, res2{};
  char c;
  Pos p;
  V v;
  while (std::cin >> p.x >> c >> p.y) {
    v.push_back(p);
  }
  const int N = v.size();

  Lines lines;
  for (int i = 1; i < N; ++i) {
    lines.emplace_back(v[i - 1], v[i]);
  }
  lines.emplace_back(v[N - 1], v[0]);

  for (int i = 0; i < N; ++i) {
    for (int j = i + 1; j < N; ++j) {
      Num a = (std::abs(v[j].x - v[i].x) + 1L) * (std::abs(v[j].y - v[i].y) + 1L);
      if (a > res1) {
        res1 = a;
      }
      if (a > res2 && check(lines, v[i], v[j])) {
        res2 = a;
      }
    }
  }
  println("1: {}", res1);
  println("2: {}", res2);
  return 0;
}
