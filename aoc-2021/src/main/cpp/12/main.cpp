#include "../utils.h"
#include <unordered_map>

using Num = int64_t;
using V = std::vector<string>;
using Graph = std::unordered_map<string, V>;

/* ------------------------------------------------------------------------ */

static Num
solve(const Graph &g, V &path, int maxSmall = 1, string n = "start", string end = "end") {
  if (n == end) {
    return 1;
  }
  Num res = 0;
  for (auto v : g.at(n)) {
    int count = 0;
    if (std::isupper(v.front()) || (count = std::count(path.begin(), path.end(), v)) < maxSmall) {
      path.push_back(v);
      res += solve(g, path, count > 0 ? 1 : maxSmall, v, end);
      path.pop_back();
    }
  }
  return res;
}

int
main() {
  Graph g;
  string line;
  while (std::getline(std::cin, line)) {
    auto it = std::find(line.begin(), line.end(), '-');
    auto e = std::string(line.begin(), it);
    auto f = std::string(it + 1, line.end());
    if (f != "start") {
      g[e].push_back(f);
    }
    if (e != "start") {
      g[f].push_back(e);
    }
  }

  V path{"start"};

  println("1: {}", solve(g, path, 1));
  println("2: {}", solve(g, path, 2));

  return 0;
}
