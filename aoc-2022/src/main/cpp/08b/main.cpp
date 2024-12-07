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
#include <unordered_map>

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
};

using Forest = std::vector<std::vector<Tree>>;

static uint
viewingDistance(const Forest& forest, int i, int j, int di, int dj) {
  uint res = 0;
  int mh = forest[i][j].h;
  i += di;
  j += dj;
  for (; 0 <= i && i < forest.size() && 0 <= j && j < forest[i].size();
      i+= di, j += dj) {
    const Tree& tree = forest[i][j];
    ++res;
    if (tree.h >= mh) {
      break;
    }
  }
  return res;
}

static uint
scenicScore(const Forest& forest, int i, int j) {
  return
    viewingDistance(forest, i, j, -1, 0) *
    viewingDistance(forest, i, j, +1, 0) *
    viewingDistance(forest, i, j, 0, -1) *
    viewingDistance(forest, i, j, 0, +1);

}

static void
print(const Forest& forest) {
  for (uint i = 0; i < forest.size(); ++i) {
    for (uint j = 0; j < forest[i].size(); ++j) {
      const Tree& tree = forest[i][j];
      std::cout << tree.h;
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
    std::transform(line.begin(), line.end(), std::back_inserter(forest.back()), [](auto c) { return Tree{c - '0'}; });
  }
  uint res = 0;
  for (uint i = 0; i < forest.size(); ++i) {
    for (uint j = 0; j < forest.front().size(); ++j) {
      res = std::max(res, scenicScore(forest, i, j));
    }
  }
  std::cout << res << "\n";
}
