#include <algorithm>
#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>

using Num = std::int64_t;
using String = std::string;
using Strings = std::vector<String>;
using Outputs = std::vector<std::pair<String, bool>>;

struct Node {
  String out;
  String left;
  String right;
  char op;
};

using Nodes = std::vector<Node>;

/* ------------------------------------------------------------------------ */

static constexpr String x(int i) { return std::format("x{:02}", i); }
static constexpr String y(int i) { return std::format("y{:02}", i); }
static constexpr String z(int i) { return std::format("z{:02}", i); }

static auto
findNode(auto& nodes, String out) {
  return std::find_if(nodes.begin(), nodes.end(), [out](auto& n) { return n.out == out; });
}

static auto
findOut(const Outputs& outs, String node) {
  return std::find_if(outs.begin(), outs.end(), [node](auto& o) {
      return o.first == node; });
}

static bool
eval(bool left, bool right, char op) {
  switch (op) {
    case 'A': return left & right;
    case 'O': return left | right;
    case 'X': return left ^ right;
  }
  return false;
}

static bool
eval(const Nodes& nodes, Outputs& outs, String node) {
  if (auto it = findOut(outs, node); it != outs.end()) {
    return it->second;
  }
  if (auto it = findNode(nodes, node); it != nodes.end()) {
    bool left = eval(nodes, outs, it->left);
    bool right = eval(nodes, outs, it->right);
    auto out = eval(left, right, it->op);
    outs.emplace_back(node, out);
    return out;
  }
  return false;
}

static int
zmax(int currMax, String candidate) {
  if (candidate[0] != 'z') {
    return currMax;
  }
  return std::max(currMax, std::stoi(candidate.substr(1)));
}

static Num
solve1(const Nodes& nodes, Outputs outs, int maxz) {
  Num res = 0;
  for (int i = 0; i <= maxz; ++i) {
    bool zv = eval(nodes, outs, z(i));
    res |= static_cast<Num>(zv) << i;
  }
  return res;
}

/* ------------------------------------------------------------------------ */

static auto
findNode(auto& nodes, String left, String right, char op) {
  return std::find_if(nodes.begin(), nodes.end(), [=](auto& n) {
    return n.op == op &&
      ((n.left == left && n.right == right) || 
      ((n.left == right && n.right == left))); });
}

static void
swapOut(Node& a, Node& b, Strings& swaps) {
  swaps.push_back(a.out);
  swaps.push_back(b.out);
  std::swap(a.out, b.out);
}

static String
solve2(Nodes nodes, int maxz) {
  Strings swaps;
  Node& z00 = *findNode(nodes, x(0), y(0), 'X'); 
  if (z00.out != z(0)) {
    Node& wrongNode = *findNode(nodes, z(0));
    swapOut(z00, wrongNode, swaps);
  }
  auto cc = findNode(nodes, x(0), y(0), 'A');
  for (int i = 1; i < maxz; ++i) {
    auto nn = findNode(nodes, x(i), y(i), 'X');
    auto zitByIns = findNode(nodes, cc->out, nn->out, 'X');
    auto zz = findNode(nodes, z(i));
    if (zitByIns == nodes.end()) {
      if (nn->out == zz->left) {
        swapOut(*cc, *findNode(nodes, zz->right), swaps);
      } else if (nn->out == zz->right) {
        swapOut(*cc, *findNode(nodes, zz->left), swaps);
      } else if (cc->out == zz->left) {
        swapOut(*nn, *findNode(nodes, zz->right), swaps);
      } else if (cc->out == zz->right) {
        swapOut(*nn, *findNode(nodes, zz->left), swaps);
      }
    } else if (zz != zitByIns) {
      swapOut(*zz, *zitByIns, swaps);
    }
    auto ss = findNode(nodes, cc->out, nn->out, 'A');
    cc = findNode(nodes, x(i), y(i), 'A');
    cc = findNode(nodes, cc->out, ss->out, 'O');
  }
  std::sort(swaps.begin(), swaps.end());
  return std::accumulate(swaps.begin() + 1, swaps.end(), swaps.front(), [](auto a, auto b) { return a + ',' + b; });
}

int
main() {
  Nodes nodes;
  Outputs outs;

  int maxz = 0;
  String line;
  while (std::getline(std::cin, line) && !line.empty()) {
    auto ss = std::stringstream(line);
    String node;
    int value;
    ss >> node >> value;
    node = node.substr(0, 3);
    maxz = zmax(maxz, node);
    outs.emplace_back(node, value);
  }

  String left, right, out, op, ig;
  while (std::cin >> left >> op >> right >> ig >> out) {
    maxz = zmax(zmax(zmax(maxz, left), right), out);
    nodes.emplace_back(out, left, right, op[0]);
  }

  std::cout << "1: " << solve1(nodes, outs, maxz) << std::endl;
  std::cout << "2:\n" << solve2(nodes, maxz) << std::endl;
}
