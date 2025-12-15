#include "../utils.h"
#include <sstream>
#include <unordered_map>
#include <unordered_set>

using Num = int64_t;
using Node = std::string;
using List = std::vector<Node>;
using Set = std::unordered_set<Node>;
using Map = std::unordered_map<Node, List>;

static void
topOrder(const Map &g, Set &nodes, List &ordered, Node n) {
  if (!nodes.contains(n)) {
    return;
  }
  auto it = g.find(n);
  if (it == g.end()) {
    return;
  }
  for (auto m : it->second) {
    topOrder(g, nodes, ordered, m);
  }
  nodes.erase(n);
  ordered.push_back(n);
}

static List
topOrder(const Map &g) {
  List res;
  Set nodes = g | views::transform([](auto e) { return e.first; }) | ranges::to<Set>();
  while (!nodes.empty()) {
    auto node = *nodes.begin();
    topOrder(g, nodes, res, node);
  }
  std::reverse(res.begin(), res.end());
  return res;
}

static Num
solve(const Map &g, const List &order, Node src, Node dst) {
  std::unordered_map<Node, Num> ways;
  ways[src] = 1;
  for (auto it = std::find(order.begin(), order.end(), src); it != order.end() && *it != dst; ++it) {
    auto w = ways[*it];
    if (w > 0) {
      for (auto m : g.find(*it)->second) {
        ways[m] += ways[*it];
      }
    }
  }
  return ways[dst];
}

/* ------------------------------------------------------------------------ */

int
main() {
  std::string line;
  Map g;

  while (std::getline(std::cin, line)) {
    std::istringstream ss{line};
    std::string from;
    ss >> from;
    List to{std::istream_iterator<std::string>(ss), std::istream_iterator<std::string>()};
    g[from.substr(0, from.size() - 1)] = to;
  }
  List order = topOrder(g);
  println("1: {}", solve(g, order, "you", "out"));
  auto dac = std::find(order.begin(), order.end(), "dac");
  auto fft = std::find(order.begin(), order.end(), "fft");
  if (dac != order.end() && fft != order.end()) {
    println("2: {}",
        solve(g, order, "svr", *std::min(dac, fft)) * solve(g, order, *std::min(dac, fft), *std::max(dac, fft)) *
            solve(g, order, *std::max(dac, fft), "out"));
  }

  return 0;
}
