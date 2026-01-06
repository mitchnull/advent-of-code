#include "../utils.h"

using Num = int64_t;

static Num
solve1(const std::vector<string> &p) {
  std::vector<bool> visited(p.size(), false);
  int pc = 0;
  Num acc = 0;
  while (!visited[pc]) {
    visited[pc] = true;
    string inst = p[pc].substr(0, 3);
    int op = std::stoi(p[pc].substr(4));
    if (inst == "acc") {
      acc += op;
    } else if (inst == "jmp") {
      pc += op - 1;
    }
    ++pc;
  }
  return acc;
}

/* ------------------------------------------------------------------------ */

int
main() {
  std::vector<string> p;
  string line;
  while (std::getline(std::cin, line)) {
    p.push_back(line);
  }

  println("1: {}", solve1(p));

  return 0;
}
