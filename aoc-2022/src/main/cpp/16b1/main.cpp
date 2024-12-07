#include <ios>
#include <iostream>
#include <cctype>
#include <limits>
#include <deque>
#include <string>
#include <vector>
#include <bitset>
#include <algorithm>
#include <ranges>
#include <charconv>
#include <numeric>
#include <memory>
#include <unordered_set>
#include <string_view>
#include <variant>
#include <sstream>
#include <set>
#include <utility>
#include <unordered_map>
#include <regex>
#include <queue>
#include <bit>

using uint8 = std::uint8_t;
using int8 = std::int8_t;
using uint32 = std::uint32_t;
using int32 = std::int32_t;
using uint64 = std::uint64_t;
using int64 = std::int64_t;

using uint = uint32;
using string = std::string;
using namespace std::literals;
using size_t = std::size_t;

static constexpr const uint TURNS = 26;
static const string START{"AA"};

struct Edge {
  string node;
  uint len = 1;

  friend auto operator<=>(const Edge&, const Edge&) = default;
};

using Edges = std::vector<Edge>;

struct Node {
  string name;
  uint rate;
  Edges edges;
  bool visited = false;
};

using Map = std::unordered_map<string, Node>;

// Valve AA has flow rate=0; tunnels lead to valves DD, II, BB
// Valve AA has flow rate=0; tunnel leads to valve DD
static void
parse(const string& line, Map& map) {
  std::regex regex("Valve (..) has flow rate=([0-9]*); tunnels? leads? to valves? (.*)");
  std::smatch match;
  if (std::regex_match(line, match, regex) && match.size() > 2) {
    Node node{match[1], (uint)std::stoi(match[2])};
    string paths = match[3];
    std::regex pathsRegex("[A-Z][A-Z]");
    for (auto it = std::sregex_token_iterator(paths.begin(), paths.end(), pathsRegex), end = std::sregex_token_iterator(); it != end; ++it) {
      node.edges.push_back({it->str()});
    }
    map[node.name] = node;
  }
}

static Edges
reachableNodes(Map map, string node) {
  Edges res;
  std::queue<Edge> queue;
  queue.push({node, 0});
  map[node].visited = true;
  while (!queue.empty()) {
    auto curr = queue.front();
    queue.pop();
    Node& n = map[curr.node];
    for (const Edge& edge: n.edges) {
      Node& next = map[edge.node];
      if (!next.visited) {
        next.visited = true;
        Edge newEdge{edge.node, curr.len + 1};
        queue.push(newEdge);
        if (next.rate > 0) {
          res.push_back(newEdge);
        }
      }
    }
  }
  std::sort(res.begin(), res.end());
  res.erase(std::unique(res.begin(), res.end(), [](const auto& a, const auto& b) { return a.node == b.node; }), res.end());
  return res;
}

static Map
simplify(const Map& map) {
  Map res;
  for (const auto& [name, node]: map) {
    if (node.rate <= 0 && name != START) {
      continue;
    }
    res[name] = {
      name,
      node.rate,
      reachableNodes(map, node.name)
    };
  }
  return res;
}

static uint
maxPressure(const Map& map, const string& node, uint turns, std::vector<string>& visited) {
  if (std::find(visited.begin(), visited.end(), node) != visited.end()) {
    return 0;
  }
  visited.push_back(node);
  --turns;
  const Node& n = map.find(node)->second;
  uint pressure = turns * n.rate;
  uint mp = 0;
  for (const auto& edge: n.edges) {
    if (turns > edge.len) {
      mp = std::max(mp, maxPressure(map, edge.node, turns - edge.len, visited));
    }
  }
  visited.pop_back();
  return pressure + mp;
}

static void
initVisited(std::vector<string>& visited, const Edges& edges, uint64 preVisited) {
  visited.clear();
  uint64 i = 1;
  for (const auto& edge: edges) {
    if (preVisited & i) {
      visited.push_back(edge.node);
    }
    i <<= 1;
  }
}

#define VERBOSE 1

int
main() {
  Map map;
  string line;
  while (std::getline(std::cin, line)) {
    parse(line, map);
  }
  map = simplify(map);
  uint res = 0;
  const auto& edges = map[START].edges;
#if VERBOSE
  std::cout << "nodes:";
  uint id = 0;
  std::cout << " " << START << "(" << id++ << ")[rate=" << map[START].rate << "]";
  for (const Edge& edge: edges) {
    const Node& node = map[edge.node];
    std::cout << " " << edge.node << "(" << id++ << ")[rate=" << node.rate << "]";
  }
  std::cout << "\n";
#endif
  std::vector<string> visited;
  for (uint64 split = 0, splitEnd = 1 << (edges.size() - 1); split < splitEnd; ++split) {
#if VERBOSE
    std::cout << '\r' << std::bit_width(splitEnd - split) << ' ';
#endif
    initVisited(visited, edges, split);
    uint me = maxPressure(map, START, TURNS + 1, visited);
    initVisited(visited, edges, ~split);
    uint nellie = maxPressure(map, START, TURNS + 1, visited);
    res = std::max(res, me + nellie);
  }
#if VERBOSE
  std::cout << "\n";
#endif
  std::cout << res << "\n";
  return 0;
}
