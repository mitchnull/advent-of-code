#include "../utils.h"
#include <sstream>

using Num = int64_t;

struct Block {
  int div, dx, dy;
};
using Blocks = std::vector<Block>;

static Num
solve1(const Blocks &blocks, Num n = 0, int z = 0, int i = 0) {
  if (i >= blocks.size()) {
    // println("@@@ n={}, z={}", n, z);
    return z == 0 ? n : -1;
  }
  auto [div, dx, dy] = blocks[i];
  for (int d = 9; d > 0; --d) {
    if (div == 26 && (z % 26 + dx) != d) {
      // println("@@@ n={}, z={}, div={}, d={}, dx={}, (z % 26 +dx)={}", n, z, div, d, dx, (z % 26 + dx));
      continue;
    }
    int nz = (div == 1) ? (z * 26 + dy + d) : (z / 26);
    auto res = solve1(blocks, n * 10 + d, nz, i + 1);
    if (res != -1) {
      return res;
    }
  }
  return -1;
}

/* ------------------------------------------------------------------------ */

int
main() {
  auto blocks = Blocks{};
  string line;
  Block b;
  while (std::getline(std::cin, line)) {
    auto ss = std::istringstream{line};
    string inst, op2;
    char op1;
    ss >> inst >> op1 >> op2;

    if (inst == "div" && op1 == 'z') {
      b.div = std::stoi(op2);
    } else if (inst == "add") {
      switch (op1) {
        case 'x': b.dx = std::atoi(op2.c_str()); break;
        case 'y': b.dy = std::atoi(op2.c_str()); break;
        case 'z': blocks.push_back(b); break;
      }
    }
  }

  println("1: {}", solve1(blocks));

  return 0;
}
