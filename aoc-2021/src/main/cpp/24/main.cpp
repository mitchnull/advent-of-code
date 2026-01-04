#include "../utils.h"
#include <sstream>

using Num = int64_t;

struct Block {
  int div, dx, dy;
};
using Blocks = std::vector<Block>;

static Num
solve(const Blocks &blocks, bool rev, Num n = 0, int z = 0, int i = 0) {
  if (i >= blocks.size()) {
    return z == 0 ? n : -1;
  }
  auto [div, dx, dy] = blocks[i];
  for (int j = 1; j < 10; ++j) {
    int d = rev ? (10 - j) : j;
    if (div == 26 && (z % 26 + dx) != d) {
      continue;
    }
    int nz = (div == 1) ? (z * 26 + dy + d) : (z / 26);
    auto res = solve(blocks, rev, n * 10 + d, nz, i + 1);
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

  println("1: {}", solve(blocks, true));
  println("2: {}", solve(blocks, false));

  return 0;
}
