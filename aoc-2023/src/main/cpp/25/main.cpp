#include "../utils.h"
#include <sstream>

using Edges = std::unordered_map<string, std::vector<string>>;
using Indexes = std::unordered_map<string, int>;
using Ints = std::vector<int>;
using Graph = std::vector<Ints>;

/* ------------------------------------------------------------------------ */


static int
condSum(int a, int b) {
  return (a < 0 || b < 0) ? -1 : a + b;
}

static int
stoerWagner(const Edges &edges, const Indexes &indexes) {
  const int N = indexes.size();
  auto g = Graph(N, Ints(N, 0));
  for (const auto &e : edges) {
    for (auto f : e.second) {
      g[indexes.at(e.first)][indexes.at(f)] = g[indexes.at(f)][indexes.at(e.first)] = 1;
    }
  }
  auto minCut = std::make_pair(std::numeric_limits<int>::max(), 0);
  auto nodes = Ints(N, 1);
  for (int e = N - 1; e > 0; --e) {
    auto w = g.front();
    int s = 0, t = 0;
    for (int j = 0; j < e; ++j) {
      w[t] = -1;
      s = t;
      t = std::max_element(w.begin(), w.end()) - w.begin();
      for (int i = 0; i < N; ++i) {
        w[i] = condSum(w[i], g[i][t]);
      }
    }
    minCut = std::min(minCut, {w[t] - g[t][t], nodes[t]});
    nodes[s] += nodes[t];
    for (int i = 0; i < N; ++i) {
      g[i][s] = g[s][i] = condSum(g[i][s], g[i][t]);
    }
    g[0][t] = -1;
  }
  return minCut.second * (N - minCut.second);
};

/* ------------------------------------------------------------------------ */

int
main() {
  string line;
  std::unordered_map<string, std::vector<string>> edges;
  std::unordered_map<string, int> indexes;
  int idx = 0;
  while (std::getline(std::cin, line)) {
    std::istringstream ss{line};
    std::string str;
    ss >> str;
    str = str.substr(0, str.size() - 1);
    if (indexes.try_emplace(str, idx).second) {
      ++idx;
    }
    auto &v = edges[str];
    while (ss >> str) {
      if (indexes.try_emplace(str, idx).second) {
        ++idx;
      }
      v.push_back(str);
    }
  }
  println("1: {}", stoerWagner(edges, indexes));

  return 0;
}
