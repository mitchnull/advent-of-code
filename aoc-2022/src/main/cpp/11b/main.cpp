#include <algorithm>
#include <bitset>
#include <cctype>
#include <charconv>
#include <deque>
#include <ios>
#include <iostream>
#include <limits>
#include <memory>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

using uint8 = std::uint8_t;
using int8 = std::int8_t;
using uint32 = std::uint32_t;
using int32 = std::int32_t;
using uint64 = std::uint64_t;
using int64 = std::int64_t;

using uint = uint32;
using string = std::string;
using namespace std::literals;

constexpr const uint N = 10000;

struct Monkey {
  int id;
  std::vector<int64> items;
  char op;
  std::optional<int> opParam;
  int mod;
  int monkeyIfTrue;
  int monkeyIfFalse;
  uint64 inspectCount = 0;

  void 
  print() const {
    std::cout
      << "id=" << id
      << ", items={";
    bool first = true;
    for (auto i: items) {
      if (first) {
        first = false;
      } else {
        std::cout << ", ";
      }
      std::cout << i;
    }
    std::cout << "}"
      << ", op=" << op
      << ", opParam=";
    if (opParam.has_value()) {
      std::cout << *opParam;
    } else {
      std::cout << "old";
    }
    std::cout << ", mod=" << mod
      << ", mt=" << monkeyIfTrue
      << ", mf=" << monkeyIfFalse
      << ", ic=" << inspectCount
      << "\n";
  }
};

using Monkeys = std::vector<Monkey>;

static Monkey
readMonkey() {
  /*
  Monkey 0:
  Starting items: 79, 98
  Operation: new = old * 19
  Test: divisible by 23
    If true: throw to monkey 2
    If false: throw to monkey 3
  */

  Monkey m;
  string str;
  std::cin >> str >> m.id;
  if (!std::cin) {
    m.id = -1;
    return m;
  }
  std::getline(std::cin, str);
  std::getline(std::cin, str);
  m.items = {};
  for (auto it = str.begin() + str.find(':') + 1, end = str.end(); it != end;) {
    auto itemEnd = std::find(++it, end, ',');
    auto itemStr = std::string(it, itemEnd);
    auto item = std::stoi(std::string(it, itemEnd));
    m.items.push_back(item);
    it = itemEnd;
  }
  std::cin >> str >> str >> str >> str >> m.op >> str;
  if (str != "old") {
    m.opParam = std::stoi(str);
  }
  std::cin >> str >> str >> str >> m.mod;
  std::cin >> str >> str >> str >> str >> str >> m.monkeyIfTrue;
  std::cin >> str >> str >> str >> str >> str >> m.monkeyIfFalse;
  m.print();
  return m;
}

static void
doRound(Monkeys& monkeys, uint64 lcm) {
  for (auto& m: monkeys) {
    for (auto i: m.items) {
      ++m.inspectCount;
      switch (m.op) {
        case '+':
          i += m.opParam.value_or(i);
          break;
        case '*':
          i *= m.opParam.value_or(i);
          break;
      }
      i %= lcm;
      monkeys[(i % m.mod) ? m.monkeyIfFalse : m.monkeyIfTrue].items.push_back(i);
    }
    m.items.clear();
  }
}

int
main() {
  Monkeys monkeys;
  uint64 lcm = 1;
  for (Monkey monkey; (monkey = readMonkey()).id != -1; ) {
    monkeys.push_back(monkey);
    lcm = std::lcm(lcm, monkey.mod);
  }
  std::cerr << "### lcm=" << lcm << "\n";
  for (uint i = 0; i < N; ++i) {
    doRound(monkeys, lcm);
  }
  std::sort(monkeys.begin(), monkeys.end(), [](const auto& a, const auto& b) { return a.inspectCount > b.inspectCount; });
  for (const auto& m : monkeys) {
    m.print();
  }
  std::cout << monkeys[0].inspectCount * monkeys[1].inspectCount << "\n";
  return 0;
}
