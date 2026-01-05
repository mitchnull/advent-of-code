#include "../utils.h"
#include <algorithm>
#include <sstream>
#include <unordered_set>

using Num = int64_t;
using Set = std::unordered_set<string>;

static const Set Required = {
    "byr",
    "iyr",
    "eyr",
    "hgt",
    "hcl",
    "ecl",
    "pid",
};

static bool
check1(const Set &required, const string &b) {
  Set found;
  string entry;
  std::istringstream ss{b};
  while (ss >> entry) {
    auto k = entry.substr(0, 3);
    if (required.contains(k)) {
      found.insert(k);
    }
  }
  return found.size() == required.size();
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

  println("1: {}", std::count_if(begin(batches), end(batches), [&](const auto &b) { return check1(Required, b); }));
  return 0;
}
