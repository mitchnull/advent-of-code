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
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
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
using size_t = std::size_t;


using Num = int64;

struct Op {
  char op;
  string left, right;
};

using Node = std::variant<Num, Op>;

using Map = std::unordered_map<string, Node>;

static std::pair<string, Node>
parse(const std::string& line) {
  std::stringstream ss{line};
  string name, left, right;
  char op;
  Num num;

  if (ss >> name >> num) {
    std::cout << "parsed1: " << name.substr(0, name.size() - 1) << ": " << num << "\n";
    return {name.substr(0, name.size() - 1), num};
  }
  ss.clear();
  ss.str(line);
  ss >> name >> left >> op >> right;
  std::cout << "parsed2: " << name.substr(0, name.size() - 1) << ": " << left << op  << right << "\n";
  return {name.substr(0, name.size() - 1), Op{op, left, right}};
}

static Num
eval(char op, Num left, Num right) {
  switch (op) {
    case '+': return left + right;
    case '-': return left - right;
    case '*': return left * right;
    case '/': return left / right;
    case '=': return left == right;
  }
  return 0;
}

static Num
eval(Map& map, const string& name) {
  const Node& node = map[name];
  if (std::holds_alternative<Num>(node)) {
    return std::get<Num>(node);
  }
  const Op& op = std::get<Op>(node);
  Num left = eval(map, op.left);
  Num right = eval(map, op.right);
  Num res = eval(op.op, left, right);
  map[name] = res;
  return res;
}

int
main() {
  Map map;

  string line;
  while (std::getline(std::cin, line)) {
    auto nn = parse(line);
    map[nn.first] = nn.second;
  }
  Num res = eval(map, "root");
  std::cout << res << "\n";
  return 0;
}
