#include <iostream>
#include <string>
#include <regex>
#include <unordered_map>

/* ------------------------------------------------------------------------ */

using std::string;
using Item = std::unordered_map<char, int>;
using Items = std::vector<Item>;
using Num = std::int64_t;
using OptString = std::optional<string>;

struct Check {
  char op;
  char var;
  int value;
  string out;

  OptString eval(const Item& item) {
    if (op == '<' && item.at(var) < value) {
      return out;
    }
    if (op == '>' && item.at(var) > value) {
      return out;
    }
    return {};
  }
};

struct Rule {
  string name;
  std::vector<Check> checks;
  string last;
  Num count = 0;

  OptString eval(const Item& item) {
    if (checks.empty()) {
      for (auto [k, v] : item) {
        count += v;
      }
      return {};
    }
    for (auto& check : checks) {
      auto out = check.eval(item);
      if ((out)) {
        return out;
      }
    }
    return last;
  }
};

using Rules = std::unordered_map<string, Rule>;

static void
eval(Rules& rules, OptString q, const Item& item) {
  while ((q)) {
    q = rules.at(*q).eval(item);
  }
}

/* ------------------------------------------------------------------------ */

int
main() {
  auto ruleRegex = std::regex("([a-z]*)[{](.*,)([a-zAR]*)[}]");
  auto checkRegex = std::regex("([xmas])([<>])([0-9]*):([a-zAR]*),");
  auto itemRegex = std::regex("([xmas])=([0-9]*),?");

  string line;
  Rules rules;
  while (std::getline(std::cin, line) && !line.empty()) {
    std::smatch sm;
    std::regex_search(line, sm, ruleRegex);
    auto rule = Rule{sm[1], {}, sm[3]};
    string checks = sm[2];
    for (auto it = std::sregex_iterator(checks.begin(), checks.end(), checkRegex), end = std::sregex_iterator(); it != end; ++it) {
      rule.checks.push_back(Check{it->str(2).front(), it->str(1).front(), std::stoi(it->str(3)), it->str(4)});
    }
    rules[rule.name] = std::move(rule);
  }
  rules["A"] = Rule{};
  rules["R"] = Rule{};

  Items items;
  while (std::getline(std::cin, line)) {
    Item item;
    for (auto it = std::sregex_iterator(line.begin(), line.end(), itemRegex), end = std::sregex_iterator(); it != end; ++it) {
      item[it->str(1).front()] = std::stoi(it->str(2));
    }
    items.push_back(std::move(item));
  }

  for (auto& item : items) {
    eval(rules, "in", item);
  }

  auto res1 = rules["A"].count;

  std::cout << res1 << "\n";
}
