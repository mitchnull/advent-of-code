#include <algorithm>
#include <iostream>
#include <numeric>
#include <regex>
#include <string>
#include <unordered_map>
#include "../utils.h"

/* ------------------------------------------------------------------------ */

using std::string;
using Item = std::unordered_map<char, int>;
using Items = std::vector<Item>;
using Num = std::int64_t;
using OptString = std::optional<string>;

constexpr const auto PROPS = { 'x', 'm', 'a', 's'};

template <>
struct std::formatter<Item> {
  constexpr auto parse(std::format_parse_context &ctx) { return ctx.begin(); }

  auto format(const Item &i, std::format_context &ctx) const {
    std::format_to(ctx.out(), "{{");
    std::string sep = "";
    for (auto p : PROPS) {
      std::format_to(ctx.out(), "{}{}={}", sep, p, i.at(p));
      sep = ",";
    }
    return std::format_to(ctx.out(), "}}");
  }
};

static Num
sumValues(const Item &item) {
  return std::transform_reduce(item.begin(), item.end(), 0, std::plus<>(), [](const auto &e) { return e.second; });
}

struct Check {
  char op;
  char var;
  int value;
  string out;

  OptString eval(const Item &item) {
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

  OptString eval(const Item &item) {
    if (checks.empty()) {
      count += sumValues(item);
      return {};
    }
    for (auto &check : checks) {
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
eval(Rules &rules, OptString q, const Item &item) {
  while ((q)) {
    q = rules.at(*q).eval(item);
  }
}

/* ------------------------------------------------------------------------ */

const int M = 4000;

struct Range {
  int b, e;

  bool isEmpty() const { return b >= e; }
  bool contains(int v) const { return b <= v && v < e; }
  Range
  shrink(char op, int p) const {
    return op == '>' ? Range{std::max(b, p + 1), e} : Range{b, std::min(e, p)};
  }
  Range
  shrinkRev(char op, int p) const {
    return op == '<' ? Range{std::max(b, p), e} : Range{b, std::min(e, p + 1)};
  }
};

template <>
struct std::formatter<Range> {
  constexpr auto parse(std::format_parse_context &ctx) { return ctx.begin(); }

  auto format(const Range &r, std::format_context &ctx) const {
    return std::format_to(ctx.out(), "[{}, {})", r.b, r.e);
  }
};

using RangeByProp = std::unordered_map<char, Range>;

static RangeByProp
allMatch() {
  return PROPS | views::transform([](auto p) { return std::make_pair(p, Range{1, M + 1}); }) | ranges::to<RangeByProp>();
}

template <>
struct std::formatter<RangeByProp> {
  constexpr auto parse(std::format_parse_context &ctx) { return ctx.begin(); }

  auto format(const RangeByProp &rp, std::format_context &ctx) const {
    std::format_to(ctx.out(), "{{");
    std::string sep = "";
    for (auto p : PROPS) {
      std::format_to(ctx.out(), "{}{}:{}", sep, p, rp.at(p));
      sep = ", ";
    }
    return std::format_to(ctx.out(), "}}");
  }
};


using Ranges = std::vector<RangeByProp>;

Ranges
process(const Rules &rules, const string &n, RangeByProp rp) {
  // println("@@@ process({}, {})...", n, rp);
  if (n == "A") {
    return {rp};
  }
  if (n == "R") {
    return {};
  }
  auto res = Ranges{};
  Rule rule = rules.at(n);
  for (auto check : rule.checks) {
    // println("@@@ checking: {}:{} {} {} -> {}", n, check.var, check.op, check.value, check.out);
    Range r = rp.at(check.var);
    Range nr = r.shrink(check.op, check.value);
    if (!nr.isEmpty()) {
      auto nrp = rp;
      nrp[check.var] = nr;
      auto rr = process(rules, check.out, nrp);
      res.insert(res.end(), rr.begin(), rr.end());
    }
    auto lr = r.shrinkRev(check.op, check.value);
    if (lr.isEmpty()) {
      return res;
    }
    rp[check.var] = lr;
  }
  auto rr = process(rules, rule.last, rp);
  res.insert(res.end(), rr.begin(), rr.end());
  return res;
}

static bool
matches(const RangeByProp &rp, const Item &item) {
  for (char c : {'x', 'm', 'a', 's'}) {
    if (!rp.at(c).contains(item.at(c))) {
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

  string line;
  Rules rules;
  while (std::getline(std::cin, line) && !line.empty()) {
    std::smatch sm;
    std::regex_search(line, sm, ruleRegex);
    auto rule = Rule{sm[1], {}, sm[3]};
    string checks = sm[2];
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

#if 0
  for (auto &item : items) {
    eval(rules, "in", item);
  }

  auto res1 = rules["A"].count;

  println("1: {}", res1);

#else
  auto matching = process(rules, "in", allMatch());
  auto res1 = std::transform_reduce(items.begin(), items.end(), 0, std::plus<>(), [&](const auto &i) {
      return (std::find_if(matching.begin(), matching.end(), [&](const auto &m) { return matches(m, i); }) != matching.end()) ? sumValues(i) : 0; });
  println("1: {}", res1);
#endif
}
