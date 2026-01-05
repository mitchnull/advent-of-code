#include "../utils.h"
#include <functional>
#include <numeric>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

using Num = std::int64_t;
using Validators = std::unordered_map<string, std::function<bool(const string &)>>;

static bool
validateRange(const string &s, Num a, Num b) {
  int v = std::stol(s);
  return a <= v && v <= b;
}

static const std::unordered_set<string> HairColors = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};

static const Validators V = {
    {"byr", [](auto s) { return validateRange(s, 1920, 2002); }},
    {"iyr", [](auto s) { return validateRange(s, 2010, 2020); }},
    {"eyr", [](auto s) { return validateRange(s, 2020, 2030); }},
    {"hgt",
        [](auto s) {
  auto unit = s.substr(s.size() - 2);
  if (unit == "cm") {
    return validateRange(s.substr(0, s.size() - 2), 150, 193);
  } else if (unit == "in") {
    return validateRange(s.substr(0, s.size() - 2), 59, 76);
  }
  return false;
}},
    {"hcl",
        [](auto s) {
  return s.size() == 7 && s[0] == '#' && s.find_first_not_of("0123456789abcdef", 1) == string::npos;
}},
    {"ecl", [](auto s) { return HairColors.contains(s); }},
    {"pid", [](auto s) { return s.size() == 9 && validateRange(s, 0, 999999999); }},
};

static std::pair<Num, Num>
check(const string &b) {
  int res1 = 0, res2 = 0;
  string entry;
  std::istringstream ss{b};
  while (ss >> entry) {
    auto k = entry.substr(0, 3);
    auto it = V.find(k);
    res1 += it != V.end();
    res2 += (it != V.end() && it->second(entry.substr(4)));
  }
  return {res1 == V.size(), res2 == V.size()};
}

/* ------------------------------------------------------------------------ */

int
main() {
  std::vector<string> batches;
  std::string batch, line;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      batches.push_back(batch);
      batch = "";
    } else {
      batch = batch + line + ' ';
    }
  }
  batches.push_back(batch);

  auto [res1, res2] = std::transform_reduce(
      begin(batches), end(batches), std::pair<Num, Num>{}, [](auto a, auto b) { return a + b; }, ::check);
  println("1: {}", res1);
  println("2: {}", res2);
  return 0;
}
