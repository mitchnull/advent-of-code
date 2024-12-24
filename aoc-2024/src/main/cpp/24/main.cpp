#include <iostream>
#include <sstream>
#include <algorithm>

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

static String x(int i) { return std::format("x{:02}", i); }
static String y(int i) { return std::format("y{:02}", i); }
static String z(int i) { return std::format("z{:02}", i); }

static auto
findNode(const Nodes& nodes, String out) {
  return std::find_if(nodes.begin(), nodes.end(), [out](auto& n) { return n.out == out; });
}

static auto
findNode(const Nodes& nodes, String left, String right, char op) {
  return std::find_if(nodes.begin(), nodes.end(), [=](auto& n) {
    return n.op == op &&
      ((n.left == left && n.right == right) || 
      ((n.left == right && n.right == left))); });
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
}
