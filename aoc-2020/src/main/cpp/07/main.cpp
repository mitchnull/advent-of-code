#include "../utils.h"
#include <algorithm>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

using Num = int64_t;
struct BS {
  string name;
  int count = 0;

  friend auto operator<=>(const BS &, const BS &) = default;
};

template <>
struct std::hash<BS> {
  std::size_t operator()(const BS &bs) const { return std::hash<string>{}(bs.name); }
};

using Set = std::unordered_set<BS>;
using SSet = std::unordered_set<string>;
using G = std::unordered_map<string, Set>;

static Num
solve1(const G &g, string bag, SSet &visited) {
  auto it = g.find(bag);
  if (it == g.end()) {
    return visited.size();
  }
  for (const auto &bs : it->second) {
    if (visited.insert(bs.name).second) {
      solve1(g, bs.name, visited);
    }
  }
  return visited.size();
}

/* ------------------------------------------------------------------------ */

int
main() {
  // light red bags contain 1 bright white bag, 2 muted yellow bags.
  G g, r;
  string line;
  while (std::getline(std::cin, line)) {
    std::istringstream ss{line};
    string c1, c2, skip;
    int count;
    ss >> c1 >> c2 >> skip >> skip;
    BS outer{c1 + ' ' + c2};
    while (ss >> count >> c1 >> c2 >> skip) {
      BS inner{c1 + ' ' + c2, count};
      g[outer.name].insert(inner);
      r[inner.name].insert(outer);
    }
  }

  SSet canContain;
  println("1: {}", solve1(r, "shiny gold", canContain));
  return 0;
}
