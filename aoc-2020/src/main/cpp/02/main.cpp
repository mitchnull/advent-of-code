#include "../utils.h"

/* ------------------------------------------------------------------------ */

int
main() {
  int a, b;
  char c, skip;
  string str;

  int res1 = 0, res2 = 0;
  while (std::cin >> a >> skip >> b >> c >> skip >> str) {
    int count = std::count(begin(str), end(str), c);
    res1 += (a <= count && count <= b);
    res2 += ((str[a - 1] == c) + (str[b - 1] == c)) == 1;
  }

  println("1: {}", res1);
  println("2: {}", res2);

  return 0;
}
