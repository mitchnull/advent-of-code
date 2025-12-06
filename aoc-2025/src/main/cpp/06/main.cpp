#include "../utils.h"
#include <sstream>

using Num = int64_t;
using Nums = std::vector<Num>;


static bool
isDataLine(std::string_view line) {
  for (auto c: line) {
    if (c == ' ') {
      continue;
    }
    return std::isdigit(c);
  }
  return false;
}

static Num
apply(char op, Num a, Num b) {
  switch (op) {
    case '+': return a + b;
    case '*': return a * b;
  }
  return 0;
}

static Num
solve1(const std::vector<Nums>& allNums, const std::vector<char>& ops) {
  Num res{};
  for (auto i = 0; i < ops.size(); ++i) {
    Num r = allNums.front()[i];
    for (auto j = 1; j < allNums.size(); ++j) {
      r = apply(ops[i], r, allNums[j][i]);
    }
    res += r;
  }
  return res;
}

/* ------------------------------------------------------------------------ */

int
main() {
  std::vector<Nums> allNums;
  std::vector<char> ops;
  std::string line;
  
  while (std::getline(std::cin, line)) {
    std::istringstream ss{line};
    if (isDataLine(line)) {
      Nums nums;
      Num n;
      while (ss >> n) {
        nums.push_back(n);
      }
      allNums.push_back(std::move(nums));
    } else {
      char c;
      while (ss >> c) {
        ops.push_back(c);
      }
    }
  }
  Num res1 = solve1(allNums, ops);

  println("1: {}", res1);
  // println("2: {}", res2);

  return 0;
}
