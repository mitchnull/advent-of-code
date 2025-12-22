#include "../utils.h"
#include <algorithm>

using Num = int64_t;

/* ------------------------------------------------------------------------ */

static char
rev(char ch) {
  switch (ch) { 
    case '(': return ')';
    case '[': return ']';
    case '{': return '}';
    case '<': return '>';
    case ')': return '(';
    case ']': return '[';
    case '}': return '{';
    case '>': return '<';
  }
  return ch;
}

static bool
pop(std::vector<char> &stack, char expected) {
  if (stack.empty()) {
    return false;
  }
  char ch = stack.back();
  stack.pop_back();
  return ch == expected;
}

static Num
score(char ch) {
  switch (ch) { 
    case '(': return 1;
    case '[': return 2;
    case '{': return 3;
    case '<': return 4;
    case ')': return 3;
    case ']': return 57;
    case '}': return 1197;
    case '>': return 25137;
  }
  return 0;
}


static std::pair<Num, Num>
solve(sv line) {
  std::vector<char> stack;
  for (char c : line) {
    switch (c) {
      case '(': case '[': case '{': case '<':
        stack.push_back(c);
        break;
      case ')': case ']': case '}': case '>':
        if (!pop(stack, rev(c))) {
          return {score(c), 0};
        }
        break;
    }
  }
  Num res2 = 0;
  for (auto it = stack.rbegin(), end = stack.rend(); it != end; ++it) {
    res2 = 5 * res2 + score(*it);
  }
  return {0, res2};
}

/* ------------------------------------------------------------------------ */

int
main() {
  std::vector<string> lines;
  string line;
  while (getline(std::cin, line)) {
    lines.push_back(line);
  }

  auto v = lines | views::transform(solve);
  Num res1 = ranges::fold_left(v | views::transform(get<0>()), Num{0}, std::plus<>());
  auto v2 = v | views::transform(get<1>()) | views::filter([](auto s) { return s > 0; }) | ranges::to<std::vector<Num>>();
  ranges::sort(v2);
  Num res2 = v2[v2.size() / 2];
  println("1: {}", res1);
  println("2: {}", res2);

  return 0;
}
