#include "../utils.h"

using Num = int64_t;
using V = std::vector<Pos>;

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
  for (int i = 0; i < N; ++i) {
    for (int j = i + 1; j < N; ++j) {
      Num a = (std::abs(v[j].x - v[i].x) + Num{1}) * (std::abs(v[j].y - v[i].y) + Num{1});
      if (a > res1) {
        res1 = a;
      }
    }
  }

  println("1: {}", res1);
  println("2: {}", res2);

  return 0;
}
