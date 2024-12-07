#include <ios>
#include <iostream>
#include <cctype>
#include <cstdint>
#include <limits>
#include <deque>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <bitset>
#include <algorithm>
#include <ranges>
#include <sstream>
#include <numeric>

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

static Num
energize(const Board& board, int i, int j, Dir dir) {
  int h = board.size();
  int w = board.front().size();
  Visited v(h, std::vector<uint>(w));
  std::vector<Step> steps;
  steps.emplace_back(i, j, dir);
  while (!steps.empty()) {
    Step s = steps.back();
    steps.pop_back();
    if (s.i < 0 || s.j < 0 || s.i >= h || s.j >= w) {
      continue;
    }
    auto& p = v[s.i][s.j];
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
          case East: case West:
            steps.emplace_back(s.i - 1, s.j, North);
            steps.emplace_back(s.i + 1, s.j, South);
            break;
        }
        break;
      case '-':
        switch (s.dir) {
          case North: case South:
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
  return energized;
}

int
main() {
  Board board;
  std::string line;
  while (std::getline(std::cin, line)) {
    board.push_back(line);
  }
  int h = board.size();
  int w = board.front().size();
  Num maxEnergized = 0;
  for (int i = 0; i < h; ++i) {
    maxEnergized = std::max(maxEnergized, energize(board, i, 0, East));
    maxEnergized = std::max(maxEnergized, energize(board, i, w - 1, West));
  }
  for (int j = 0; j < w; ++j) {
    maxEnergized = std::max(maxEnergized, energize(board, 0, j, South));
    maxEnergized = std::max(maxEnergized, energize(board, h - 1, j, North));
  }
  std::cout << maxEnergized << "\n";
  return 0;
}
