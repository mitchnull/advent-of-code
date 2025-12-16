#include "../utils.h"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <regex>
#include <string>
#include <unordered_map>

/* ------------------------------------------------------------------------ */

using Num = std::int64_t;
using std::string;

enum Props {
  X, M, A, S
};

constexpr const auto PROPS = {X, M, A, S};

using Item = std::array<int, PROPS.size()>;
using Items = std::vector<Item>;

struct Check {
  char op;
  char var;
  int value;
  std::string out;
};

struct Rule {
  std::string name;
  std::vector<Check> checks;
  std::string last;
};

using Rules = std::unordered_map<std::string, Rule>;

struct Range {
  int b = 1, e = 4001;

  bool isEmpty() const { return b >= e; }
  bool contains(int v) const { return b <= v && v < e; }
  Range shrink(char op, int p) const { return op == '>' ? Range{std::max(b, p + 1), e} : Range{b, std::min(e, p)}; }
  Range shrinkRev(char op, int p) const { return op == '<' ? Range{std::max(b, p), e} : Range{b, std::min(e, p + 1)}; }
  int size() const { return std::max(e - b, 0); };
};

using RangeByProp = std::unordered_map<char, Range>;
using Ranges = std::vector<RangeByProp>;

/* ------------------------------------------------------------------------ */

void
process(const Rules &rules, const std::string &n, RangeByProp rp, Ranges &res) {
  if (n == "A") {
    res.push_back(rp);
    return;
  }
  if (n == "R") {
    return;
  }
  Rule rule = rules.at(n);
  for (auto check : rule.checks) {
    Range r = rp.at(check.var);
    Range nr = r.shrink(check.op, check.value);
    if (!nr.isEmpty()) {
      auto nrp = rp;
      nrp[check.var] = nr;
      process(rules, check.out, nrp, res);
    }
    auto lr = r.shrinkRev(check.op, check.value);
    if (lr.isEmpty()) {
      return;
    }
    rp[check.var] = lr;
  }
  process(rules, rule.last, rp, res);
}

static bool
matches(const RangeByProp &rp, const Item &item) {
  for (char p : PROPS) {
    if (!rp.at(p).contains(item.at(p))) {
      return false;
    }
  }
  return true;
}

/* ------------------------------------------------------------------------ */

int
main() {
  auto ruleRegex = std::regex("([a-z]*)[{](.*,)([a-zAR]*)[}]");
  auto checkRegex = std::regex("([xmas])([<>])([0-9]*):([a-zAR]*),");
  auto itemRegex = std::regex("([xmas])=([0-9]*),?");

  std::string line;
  Rules rules;
  while (std::getline(std::cin, line) && !line.empty()) {
    std::smatch sm;
    std::regex_search(line, sm, ruleRegex);
    auto rule = Rule{sm[1], {}, sm[3]};
    std::string checks = sm[2];
    for (auto it = std::sregex_iterator(checks.begin(), checks.end(), checkRegex), end = std::sregex_iterator();
        it != end;
        ++it) {
      rule.checks.push_back(Check{it->str(2).front(), it->str(1).front(), std::stoi(it->str(3)), it->str(4)});
    }
    rules[rule.name] = std::move(rule);
  }
  rules["A"] = Rule{"A"};
  rules["R"] = Rule{"R"};

  Items items;
  while (std::getline(std::cin, line)) {
    Item item;
    for (auto it = std::sregex_iterator(line.begin(), line.end(), itemRegex), end = std::sregex_iterator(); it != end;
        ++it) {
      item[it->str(1).front()] = std::stoi(it->str(2));
    }
    items.push_back(std::move(item));
  }

  RangeByProp init = PROPS | views::transform([](auto p) { return std::make_pair(p, Range{}); }) | ranges::to<RangeByProp>();
  Ranges matching;
  process(rules, "in", init, matching);

  Num res1 = std::transform_reduce(items.begin(), items.end(), Num{0}, std::plus<>(), [&](const auto &i) {
      return (std::find_if(matching.begin(), matching.end(), [&](const auto &m) { return matches(m, i); }) !=
      matching.end()) ? std::transform_reduce(i.begin(), i.end(), 0, std::plus<>(), [](const auto &e) { return e.second; }) : 0; });
  Num res2 = std::transform_reduce(matching.begin(), matching.end(), Num{0}, std::plus<>(), [](const auto &m) {
    return std::transform_reduce(
        m.begin(), m.end(), Num{1}, std::multiplies<>(), [](const auto &e) { return e.second.size(); });
  });

  println("1: {}", res1);
  println("2: {}", res2);
  return 0;
}
