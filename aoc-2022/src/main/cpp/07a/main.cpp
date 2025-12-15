#include <algorithm>
#include <bitset>
#include <cctype>
#include <charconv>
#include <deque>
#include <ios>
#include <iostream>
#include <limits>
#include <memory>
#include <numeric>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>

using uint8 = std::uint8_t;
using int8 = std::int8_t;
using uint32 = std::uint32_t;
using int32 = std::int32_t;
using uint64 = std::uint64_t;
using int64 = std::int64_t;

using uint = uint32;
using string = std::string;

constexpr const uint MAX_SIZE = 100000;

enum class Type {
  Dir,
  File,
};

struct Node;
using NodePtr = std::unique_ptr<Node>;
using NodePtrMap = std::unordered_map<string, NodePtr>;

struct Node {
  string name;
  Type type;
  uint size;
  Node *parent;
  NodePtrMap children;

  Node(string name, Type type, uint size, Node *parent, NodePtrMap children)
      : name(std::move(name)), type(type), size(size), parent(parent), children(std::move(children)) {}

  Node *add(NodePtr node) {
    if (node->size != 0) {
      for (auto p = this; p != nullptr; p = p->parent) {
        p->size += node->size;
      }
    }
    return children.emplace(node->name, std::move(node)).first->second.get();
  }
};

static NodePtr
makeFile(const string &name, Node *parent, uint size) {
  return std::make_unique<Node>(name, Type::File, size, parent, NodePtrMap{});
}

static NodePtr
makeDir(const string &name, Node *parent) {
  return std::make_unique<Node>(name, Type::Dir, 0, parent, NodePtrMap{});
}

constexpr const std::string_view CMD_CD = "$ cd ";
constexpr const uint LEN_CD = CMD_CD.size();

static Node *
parse(const string &line, Node &root, Node *cwd) {
  if (line.starts_with(CMD_CD)) {
    string name = line.substr(LEN_CD);
    if (name == "/") {
      return &root;
    } else if (name == "..") {
      return cwd->parent;
    } else {
      auto it = cwd->children.find(name);
      if (it != cwd->children.end()) {
        return it->second.get();
      }
      return cwd->add(makeDir(name, cwd));
    }
  } else if (line[0] == '$') {
    // no-op
  } else {
    auto p = line.find(' ');
    string size = line.substr(0, p);
    auto name = line.substr(p + 1);
    if (cwd->children.contains(name)) {
      return cwd;
    }
    if (size == "dir") {
      cwd->add(makeDir(name, cwd));
    } else {
      cwd->add(makeFile(name, cwd, std::stoi(size)));
    }
  }
  return cwd;
}

static void
print(const Node &node, std::string indent = "") {
  std::cout << indent << node.name << ", type: " << (node.type == Type::Dir ? "dir" : "file") << ", size: " << node.size
            << "\n";
  for (const auto &kv : node.children) {
    print(*kv.second, indent + "  ");
  }
}

static uint
sumDirs(const Node &dir) {
  uint res = 0;
  for (const auto &kv : dir.children) {
    const Node &node = *kv.second;
    if (node.type == Type::Dir) {
      res += sumDirs(node);
      if (node.size <= MAX_SIZE) {
        res += node.size;
      }
    }
  }
  return res;
}

int
main() {
  Node root = Node{"/", Type::Dir, 0, nullptr, {}};
  Node *cwd = &root;
  string line;
  while (std::getline(std::cin, line)) {
    cwd = parse(line, root, cwd);
  }
  std::cout << sumDirs(root) << "\n";
  return 0;
}
