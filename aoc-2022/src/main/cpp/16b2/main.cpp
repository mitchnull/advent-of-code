#include <algorithm>
#include <bit>
#include <bitset>
#include <cctype>
#include <charconv>
#include <deque>
#include <ios>
#include <iostream>
#include <limits>
#include <memory>
#include <numeric>
#include <queue>
#include <ranges>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

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

  friend auto operator<=>(const Edge &, const Edge &) = default;
};

using Edges = std::vector<Edge>;

struct Node {
  string name;
  uint rate;
  Edges edges;
  bool visited = false;
};

using Map = std::unordered_map<string, Node>;

struct IEdge {
  uint node;
  uint len;
  friend auto operator<=>(const IEdge &, const IEdge &) = default;
};

using IEdges = std::vector<IEdge>;

struct INode {
  uint id;
  string name;
  uint rate;
  IEdges edges;
};

using INodes = std::vector<INode>;

// Valve AA has flow rate=0; tunnels lead to valves DD, II, BB
// Valve AA has flow rate=0; tunnel leads to valve DD
static void
parse(const string &line, Map &map) {
  std::regex regex("Valve (..) has flow rate=([0-9]*); tunnels? leads? to valves? (.*)");
  std::smatch match;
  if (std::regex_match(line, match, regex) && match.size() > 2) {
    Node node{match[1], (uint)std::stoi(match[2])};
    string paths = match[3];
    std::regex pathsRegex("[A-Z][A-Z]");
    for (auto it = std::sregex_token_iterator(paths.begin(), paths.end(), pathsRegex),
              end = std::sregex_token_iterator();
        it != end;
        ++it) {
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
    Node &n = map[curr.node];
    for (const Edge &edge : n.edges) {
      Node &next = map[edge.node];
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
  res.erase(
      std::unique(res.begin(), res.end(), [](const auto &a, const auto &b) { return a.node == b.node; }), res.end());
  return res;
}

static uint
findId(const string &node, const std::vector<string> &nodes) {
  return std::find(nodes.begin(), nodes.end(), node) - nodes.begin();
}

static IEdges
convertToIEdges(const Edges &edges, const std::vector<string> &nodes) {
  IEdges res;
  res.reserve(edges.size());
  for (const auto &edge : edges) {
    uint id = findId(edge.node, nodes);
    res.push_back({id, edge.len});
  }
  return res;
}

static std::vector<string>
nodeNames(const Map &map) {
  std::vector<string> res{START};
  for (const auto &[name, node] : map) {
    if (node.rate <= 0 || name == START) {
      continue;
    }
    res.push_back(name);
  }
  std::sort(res.begin() + 1, res.end());
  return res;
}

static INodes
simplify(const Map &map) {
  INodes res;
  auto nodes = nodeNames(map);
  for (const auto &nodeName : nodes) {
    const auto &[name, node] = *map.find(nodeName);
    uint id = findId(name, nodes);
    res.push_back({
        id,
        name,
        node.rate,
        convertToIEdges(reachableNodes(map, name), nodes),
    });
  }
  return res;
}

using Visited = std::bitset<64>;

static uint
maxPressure(const INodes &nodes, uint node, uint turns, Visited &visited) {
  if (visited[node]) {
    return 0;
  }
  visited[node] = true;
  --turns;
  const INode &n = nodes[node];
  uint pressure = turns * n.rate;
  uint mp = 0;
  for (const auto &edge : n.edges) {
    if (turns > edge.len) {
      mp = std::max(mp, maxPressure(nodes, edge.node, turns - edge.len, visited));
    }
  }
  visited[node] = false;
  return pressure + mp;
}

int
main() {
  Map map;
  string line;
  while (std::getline(std::cin, line)) {
    parse(line, map);
  }
  auto nodes = simplify(map);
  uint res = 0;
  Visited visited;
  for (uint64 split = 0, splitEnd = 1 << (nodes.size() - 2); split < splitEnd; ++split) {
    visited = split << 1;
    uint me = maxPressure(nodes, 0, TURNS + 1, visited);
    visited = ~split << 1;
    uint nellie = maxPressure(nodes, 0, TURNS + 1, visited);
    res = std::max(res, me + nellie);
  }
  std::cout << res << "\n";
  return 0;
}
