#include "../utils.h"

using Num = int64_t;

static std::pair<Num, bool>
solve1(const std::vector<string> &p) {
  std::vector<bool> visited(p.size(), false);
  int pc = 0;
  Num acc = 0;
  while (pc < p.size() && !visited[pc]) {
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
  return {acc, pc >= visited.size()};
}

static Num
solve2(std::vector<string> p) {
  for (int i = 0; i < p.size(); ++i) {
    if (p[i].substr(0, 3) == "jmp") {
      string bak = p[i];
      p[i] = "nop 0";
      auto [res, success] = solve1(p);
      if (success) {
        return res;
      }
      p[i] = bak;
    }
  }
  return -1;
}

/* ------------------------------------------------------------------------ */

int
main() {
  std::vector<string> p;
  string line;
  while (std::getline(std::cin, line)) {
    p.push_back(line);
  }

  println("1: {}", solve1(p).first);
  println("1: {}", solve2(p));

  return 0;
}
