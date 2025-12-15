#include "../utils.h"
#include <algorithm>
#include <numeric>
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

#if HAS_FOLD_LEFT_FIRST
static Num
solve1(const std::vector<Nums>& allNums, const std::vector<char>& ops) {
  return *ranges::fold_left_first(views::iota(0UZ, ops.size()) | views::transform([&](auto i) {
    return *ranges::fold_left_first(allNums | views::transform([i](auto &ns) { return ns[i]; }), [&](auto a, auto b) { return calc(ops[i], a, b); });
  }), std::plus<>{});
}
#else
static Num
solve1(const std::vector<Nums>& allNums, const std::vector<char>& ops) {
  return ranges::fold_left(views::iota(0UZ, ops.size()) | views::transform([&](auto i) {
    auto v = allNums | views::transform([i](auto& ns) { return ns[i]; });
    return std::reduce(v.begin() + 1, v.end(), v.front(), [&](auto a, auto b) { return calc(ops[i], a, b); });
  }), Num{}, std::plus<>{});
}
#endif

static Num
solve2(const std::vector<std::string>& dataLines, const std::vector<char>& ops) {
  Num res{}, r{};
  auto it = ops.begin();
  for (int i = 0, e = dataLines.front().size(); i < e; ++i) {
    Num n = ranges::fold_left(dataLines | views::transform([&](auto &d) { return d[i]; }) | views::filter(isdigit),
        Num{}, [](auto r, auto a) { return r * 10 + a - '0'; });
    if (n != 0) {
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
