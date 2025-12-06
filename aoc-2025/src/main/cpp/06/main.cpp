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
calc(char op, Num a, Num b) {
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
      r = calc(ops[i], r, allNums[j][i]);
    }
    res += r;
  }
  return res;
}

static Num
solve2(const std::vector<std::string>& dataLines, const std::vector<char>& ops) {
  Num res{}, r{};
  auto it = ops.begin();
  for (int i = 0, e = dataLines.front().size(); i < e; ++i) {
    std::string data;
    for (const auto& dataLine : dataLines) {
      data += dataLine[i];
    }
    if (isDataLine(data)) {
      Num n = std::stol(data);
      r = r == 0 ? n : calc(*it, r, n);
    } else {
      res += r;
      r = 0;
      ++it;
    }
  }
  return res + r;
}

/* ------------------------------------------------------------------------ */

int
main() {
  std::vector<std::string> dataLines;
  std::vector<Nums> allNums;
  std::vector<char> ops;
  std::string line;
  
  while (std::getline(std::cin, line)) {
    std::istringstream ss{line};
    if (isDataLine(line)) {
      dataLines.push_back(line);
      allNums.emplace_back(std::istream_iterator<Num>(ss), std::istream_iterator<Num>());
    } else {
      ops = {std::istream_iterator<char>(ss), std::istream_iterator<char>()};
    }
  }
  println("1: {}", solve1(allNums, ops));
  println("2: {}", solve2(dataLines, ops));

  return 0;
}
