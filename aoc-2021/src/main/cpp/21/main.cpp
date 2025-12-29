#include "../utils.h"

using Num = int64_t;

static Num
solve1(Num p1, Num p2) {
  Num count = 0, die = 0, s1 = 0, s2 = 0;
  while (true) {
    for (int i = 0; i < 3; ++i) {
      die = (die + 1) % 100;
      p1 = (p1 + die - 1) % 10 + 1;
      ++count;
    }
    s1 += p1;
    if (s1 >= 1000) {
      return s2 * count;
    }
    std::swap(p1, p2);
    std::swap(s1, s2);
  }
}

/* ------------------------------------------------------------------------ */

int
main() {
  //  Player 1 starting position: 4
  string line;
  Num p1, p2;
  std::getline(std::cin, line);
  p1 = std::stoi(line.substr(string{"Player 1 starting position: "}.size()));
  std::getline(std::cin, line);
  p2 = std::stoi(line.substr(string{"Player 2 starting position: "}.size()));

  println("1: {}", solve1(p1, p2));

  return 0;
}
