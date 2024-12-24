#include <iostream>
#include <unordered_map>
#include <sstream>

using Num = std::int64_t;
using String = std::string;
using Outputs = std::unordered_map<String, bool>;

struct Input {
  String left;
  String right;
  char op;
};

using Inputs = std::unordered_map<String, Input>;

/* ------------------------------------------------------------------------ */

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
eval(Inputs& ins, Outputs& outs, String node) {
  if (auto it = outs.find(node); it != outs.end()) {
    return it->second;
  }
  if (auto it = ins.find(node); it != ins.end()) {
    bool left = eval(ins, outs, it->second.left);
    bool right = eval(ins, outs, it->second.right);
    auto out = eval(left, right, it->second.op);
    return outs[node] = out;
  }
  return false;
}

static String
zmax(String currMax, String candidate) {
  if (candidate[0] == 'z' && candidate > currMax) {
    return candidate;
  }
  return currMax;
}

int
main() {
  Inputs ins;
  Outputs outs;

  String maxz;
  String line;
  while (std::getline(std::cin, line) && !line.empty()) {
    auto ss = std::stringstream(line);
    String node;
    int value;
    ss >> node >> value;
    node = node.substr(0, 3);
    maxz = zmax(maxz, node);
    outs[node] = value;
  }

  String left, right, out, op, ig;
  while (std::cin >> left >> op >> right >> ig >> out) {
    maxz = zmax(maxz, left);
    maxz = zmax(maxz, right);
    maxz = zmax(maxz, out);
    ins[out] = {left, right, op[0]};
  }

  Num res1 = 0;
  String z;
  for (int i = 0; z != maxz; ++i) {
    z = std::format("z{:02}", i);
    bool zv = eval(ins, outs, z);
    res1 |= static_cast<Num>(zv) << i;
  }
  std::cout << "1: " << res1 << std::endl;
}
