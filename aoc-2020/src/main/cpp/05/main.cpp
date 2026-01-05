#include "../utils.h"
#include <algorithm>

using Num = int64_t;

static Num
bs(sv bsp, Num l, Num r) {
  if (bsp.empty()) {
    return l;
  }
  return (bsp[0] == 'L' || bsp[0] == 'F') ? bs(bsp.substr(1), l, (l + r) / 2) : bs(bsp.substr(1), (l + r) / 2, r);
};

static Num
id(string bsp) {
  std::println("row({}): {}, col({}): {}",
      bsp.substr(0, 7),
      bs(bsp.substr(0, 7), 0, 128),
      bsp.substr(7),
      bs(bsp.substr(7), 0, 8));
  return 8 * bs(bsp.substr(0, 7), 0, 128) + bs(bsp.substr(7), 0, 8);
}

/* ------------------------------------------------------------------------ */

int
main() {
  string bsp;
  std::vector<Num> ids;
  while (std::cin >> bsp) {
    ids.push_back(id(bsp));
  }
  ranges::sort(ids);
  println("1: {}", ids.back());
  for (auto i = begin(ids), e = end(ids) - 1; i < e; ++i) {
    if (*i + 1 != *(i + 1)) {
      println("2: {}", *i + 1);
      break;
    }
  }

  return 0;
}
