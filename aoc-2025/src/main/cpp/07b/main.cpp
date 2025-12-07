#include "../utils.h"
#include <algorithm>

using Num = int64_t;

/* ------------------------------------------------------------------------ */

int
main() {
  std::string line;
  std::getline(std::cin, line);
  auto ways = line | views::transform([](auto c) { return c == 'S'; }) | ranges::to<std::vector<Num>>();
  Num splits{};
  while (std::getline(std::cin, line)) {
    for (int x = 0; x < line.size(); ++x) {
      if (line[x] == '^') {
        splits += ways[x] != 0;
        if (x > 0) {
          ways[x - 1] += ways[x];
        }
        if (x < ways.size() - 1) {
          ways[x + 1] += ways[x];
        }
        ways[x] = 0;
      }
    }
  }

  println("1: {}", splits);
  println("2: {}", ranges::fold_left(ways, Num{}, std::plus<>{}));

  return 0;
}
