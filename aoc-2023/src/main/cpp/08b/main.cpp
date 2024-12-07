#include <ios>
#include <iostream>
#include <cctype>
#include <cstdint>
#include <limits>
#include <deque>
#include <numeric>
#include <string>
#include <vector>
#include <map>
#include <bitset>
#include <algorithm>
#include <ranges>
#include <unordered_map>
#include <sstream>
#include <charconv>

using uint8 = std::uint8_t;
using int8 = std::int8_t;
using uint32 = std::uint32_t;
using int32 = std::int32_t;
using uint64 = std::uint64_t;
using int64 = std::int64_t;

using uint = uint32;

using Num = int64;

namespace ranges = std::ranges;
namespace views = ranges::views;

using Node = std::unordered_map<char, std::string>;
using Nodes = std::unordered_map<std::string, Node>;

struct Z {
  Num mod;
  std::vector<Num> offsets;
};

struct Pos {
  Num count;
  std::string node;

  friend auto operator<=>(const Pos&, const Pos&) = default;
};

static char
ignore(char c) {
  if (!isspace(c) && !isalnum(c)) {
    return ' ';
  }
  return c;
}

static bool
areWeThereYet(const std::vector<std::string>& nodes) {
  for (const auto& node: nodes) {
    if (node.back() != 'Z') {
      return false;
    }
  }
  return true;
}

static Z
findZ(const std::string& steps, Nodes& nodes, const std::string& node) {
  std::vector<Pos> visited;
  Pos curr = {0, node};
  while (true) {
    char step = steps[curr.count++ % steps.size()];
    curr.node = nodes[curr.node][step];
    if (curr.node.back() == 'Z') {
      auto it = std::find_if(visited.begin(), visited.end(), [&](const auto& v) { return v.node == curr.node && (curr.count - v.count) % steps.size() == 0; });
      if (it != visited.end()) {
        Num mod = curr.count - it->count;
        std::vector<Num> offsets;
        std::transform(visited.begin(), visited.end(), std::back_inserter(offsets), [](const auto &v) { return v.count; });
        return {mod, offsets};
      }
      visited.push_back(curr);
    }
  }
}

int
main() {
  std::string line, steps;
  std::getline(std::cin, steps);
  Nodes nodes;
  while (std::getline(std::cin, line)) {
    std::transform(line.begin(), line.end(), line.begin(), ignore);
    std::string start, left, right;
    std::stringstream ss(line);
    if (ss >> start >> left >> right) {
      nodes[start] = {{'L', left}, {'R', right}};
    }
  }

  std::vector<std::string> currNodes;
  for (const auto& node: nodes) {
    if (node.first.back() == 'A') {
      currNodes.push_back(node.first);
    }
  }

  Num lcm = 1;
  for (const auto& node: currNodes) {
    Z z = findZ(steps, nodes, node);
    if (z.offsets.size() == 1 && z.offsets.front() == z.mod) {
      lcm = std::lcm(lcm, z.mod);
    } else {
      lcm = 0;
    }
  }

  if (lcm) {
    std::cout << lcm << "\n";
    return 0;
  }

  int count = 0;
  while (!areWeThereYet(currNodes)) {
    char step = steps[count++ % steps.size()];
    std::transform(currNodes.begin(), currNodes.end(), currNodes.begin(), [&](const auto& node) {
      return nodes[node][step];
    });
  }
  std::cout << count << "\n";
  return 0;
}
