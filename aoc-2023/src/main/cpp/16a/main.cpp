#include <algorithm>
#include <bitset>
#include <cctype>
#include <cstdint>
#include <deque>
#include <ios>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <ranges>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using Num = std::uint64_t;
using std::swap;

using Board = std::vector<std::string>;
using Visited = std::vector<std::vector<uint>>;

enum Dir {
  North = 1,
  East = 2,
  South = 4,
  West = 8,
};

struct Step {
  int i, j;
  Dir dir;
};

int
main() {
  Board board;
  Visited v;
  std::vector<Step> steps;
  std::string line;
  while (std::getline(std::cin, line)) {
    board.push_back(line);
    v.push_back(std::vector<uint>(line.size()));
  }
  steps.emplace_back(0, 0, East);
  int h = board.size();
  int w = board.front().size();
  while (!steps.empty()) {
    Step s = steps.back();
    steps.pop_back();
    if (s.i < 0 || s.j < 0 || s.i >= h || s.j >= w) {
      continue;
    }
    auto &p = v[s.i][s.j];
    if (p & s.dir) {
      continue;
    }
    p |= s.dir;
    switch (board[s.i][s.j]) {
      case '/':
        switch (s.dir) {
          case North: steps.emplace_back(s.i, s.j + 1, East); break;
          case East: steps.emplace_back(s.i - 1, s.j, North); break;
          case South: steps.emplace_back(s.i, s.j - 1, West); break;
          case West: steps.emplace_back(s.i + 1, s.j, South); break;
        }
        break;
      case '\\':
        switch (s.dir) {
          case North: steps.emplace_back(s.i, s.j - 1, West); break;
          case East: steps.emplace_back(s.i + 1, s.j, South); break;
          case South: steps.emplace_back(s.i, s.j + 1, East); break;
          case West: steps.emplace_back(s.i - 1, s.j, North); break;
        }
        break;
      case '|':
        switch (s.dir) {
          case North: steps.emplace_back(s.i - 1, s.j, North); break;
          case South: steps.emplace_back(s.i + 1, s.j, South); break;
          case East:
          case West:
            steps.emplace_back(s.i - 1, s.j, North);
            steps.emplace_back(s.i + 1, s.j, South);
            break;
        }
        break;
      case '-':
        switch (s.dir) {
          case North:
          case South:
            steps.emplace_back(s.i, s.j + 1, East);
            steps.emplace_back(s.i, s.j - 1, West);
            break;
          case East: steps.emplace_back(s.i, s.j + 1, East); break;
          case West: steps.emplace_back(s.i, s.j - 1, West); break;
        }
        break;
      default:
        switch (s.dir) {
          case North: steps.emplace_back(s.i - 1, s.j, North); break;
          case South: steps.emplace_back(s.i + 1, s.j, South); break;
          case East: steps.emplace_back(s.i, s.j + 1, East); break;
          case West: steps.emplace_back(s.i, s.j - 1, West); break;
        }
        break;
    }
  }
  Num energized = 0;
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      if (v[i][j]) {
        ++energized;
      }
    }
  }
  std::cout << energized << "\n";
  return 0;
}
