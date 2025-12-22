#include "../utils.h"
#include <unordered_map>

using Num = int64_t;
using V = std::vector<string>;
using Graph = std::unordered_map<string, V>;

/* ------------------------------------------------------------------------ */

static Num
solve1(const Graph &g, V &path, string n = "start", string end = "end") {
  if (n == end) {
    return 1;
  }
  Num res = 0;
  for (auto v : g.at(n)) {
    if (std::isupper(v.front()) || std::find(path.begin(), path.end(), v) == path.end()) {
      path.push_back(v);
      res += solve1(g, path, v, end);
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
    g[e].push_back(f);
    if (e != "start") {
      g[f].push_back(e);
    }
  }

  V path{"start"};

  println("1: {}", solve1(g, path));

  return 0;
}
