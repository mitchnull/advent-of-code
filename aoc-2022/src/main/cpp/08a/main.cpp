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

struct Tree {
  int h;
  bool visible;
};

using Forest = std::vector<std::vector<Tree>>;

static uint
countVisible(Forest &forest, int i, int j, int di, int dj) {
  uint res = 0;
  for (int mh = -1; 0 <= i && i < forest.size() && 0 <= j && j < forest[i].size(); i += di, j += dj) {
    Tree &tree = forest[i][j];
    if (tree.h > mh) {
      mh = tree.h;
      if (!tree.visible) {
        tree.visible = true;
        ++res;
      }
    }
  }
  return res;
}

static void
print(const Forest &forest) {
  for (uint i = 0; i < forest.size(); ++i) {
    for (uint j = 0; j < forest[i].size(); ++j) {
      const Tree &tree = forest[i][j];
      if (tree.visible) {
        std::cout << "\033[1m" << tree.h << "\033[0m";
      } else {
        std::cout << tree.h;
      }
    }
    std::cout << "\n";
  }
}

int
main() {
  string line;
  Forest forest;
  while (std::cin >> line) {
    forest.push_back({});
    std::transform(
        line.begin(), line.end(), std::back_inserter(forest.back()), [](auto c) { return Tree{c - '0', false}; });
  }
  uint res = 0;
  for (uint i = 0; i < forest.size(); ++i) {
    res += countVisible(forest, i, 0, 0, +1);
    res += countVisible(forest, i, forest[i].size() - 1, 0, -1);
  }
  for (uint j = 0; j < forest.front().size(); ++j) {
    res += countVisible(forest, 0, j, +1, 0);
    res += countVisible(forest, forest.size() - 1, j, -1, 0);
  }
  std::cout << res << "\n";
}
