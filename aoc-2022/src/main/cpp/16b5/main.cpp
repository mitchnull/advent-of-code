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

static uint
findId(const string &node, const std::vector<string> &nodes) {
  return std::find(nodes.begin(), nodes.end(), node) - nodes.begin();
}

static std::vector<string>
nodeNames(const Map &map) {
  std::vector<string> res{START};
  for (const auto &[name, node] : map) {
    if (name != START) {
      res.push_back(name);
    }
  }
  return res;
}

static INodes
simplify(const Map &map) {
  INodes res;
  auto nodes = nodeNames(map);
  uint n = nodes.size();
  std::vector<uint> dist(n * n, TURNS);
  for (const auto &nodeName : nodes) {
    const auto &[name, node] = *map.find(nodeName);
    uint i = findId(name, nodes);
    dist[i * n + i] = 0;
    for (const auto &edge : node.edges) {
      uint j = findId(edge.node, nodes);
      if (j >= n) {
        exit(0);
      }
      dist[i * n + j] = edge.len;
    }
  }
  for (uint k = 0; k < n; ++k) {
    for (uint i = 0; i < n; ++i) {
      for (uint j = 0; j < n; ++j) {
        if (dist[i * n + j] > dist[i * n + k] + dist[k * n + j]) {
          dist[i * n + j] = dist[i * n + k] + dist[k * n + j];
        }
      }
    }
  }
  std::vector<uint> mapping{0};
  uint m = 0;
  for (uint i = 1; i < n; ++i) {
    mapping.push_back(map.find(nodes[i])->second.rate > 0 ? ++m : n);
  }
  for (uint i = 0; i < n; ++i) {
    uint mi = mapping[i];
    if (mi == n) {
      continue;
    }
    INode iNode = {map.find(nodes[i])->second.rate, {}};
    for (uint j = 1; j < n; ++j) {
      uint mj = mapping[j];
      if (mj == n) {
        continue;
      }
      uint len = dist[i * n + j];
      if (i != j && len < TURNS) {
        iNode.edges.push_back({mj, len});
      }
    }
    res.push_back(std::move(iNode));
  }
  return res;
}

using Pressures = std::vector<uint>;

void
maxPressure(const INodes &nodes, uint node, uint turns, uint pressure, size_t visited, Pressures &pressures) {
  uint vbit = node > 0 ? (1 << (node - 1)) : 0;
  if (visited & vbit) {
    return;
  }
  visited |= vbit;
  --turns;
  const INode &n = nodes[node];
  pressure += turns * n.rate;
  uint &cached = pressures[visited];
  cached = std::max(cached, pressure);
  for (const auto &edge : n.edges) {
    if (turns > edge.len) {
      maxPressure(nodes, edge.node, turns - edge.len, pressure, visited, pressures);
    }
  }
}

int
main() {
  Map map;
  string line;
  while (std::getline(std::cin, line)) {
    parse(line, map);
  }
  auto nodes = simplify(map);
  size_t visitedEnd = 1 << (nodes.size() - 1);
  Pressures pressures(visitedEnd, 0);
  maxPressure(nodes, 0, TURNS + 1, 0, 0, pressures);
  std::vector<size_t> idxs;
  for (size_t i = 0; i < visitedEnd; ++i) {
    if (pressures[i] > 0) {
      idxs.push_back(i);
    }
  }
  uint res = 0;
  for (auto me = idxs.cbegin(), end = idxs.cend(); me != end; ++me) {
    for (auto nellie = me + 1; nellie != end; ++nellie) {
      if (!(*me & *nellie)) {
        res = std::max(res, pressures[*me] + pressures[*nellie]);
      }
    }
  }
  std::cout << res << "\n";
  return 0;
}
