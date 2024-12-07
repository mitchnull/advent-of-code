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

namespace ranges = std::ranges;
namespace views = ranges::views;

using Board = std::vector<std::string>;

static void
tilt(Board& board) {
  int height = board.size();
  int width = board.front().size();
  for (int i = 1; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      if (board[i][j] == 'O') {
        int k = i;
        while (--k >= 0 && board[k][j] == '.') {
          continue;
        }
        if (++k != i) {
          swap(board[i][j], board[k][j]);
        }
      }
    }
  }
}

static Num
calcLoad(const Board& board) {
  int height = board.size();
  int width = board.front().size();
  Num sum = 0;
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      if (board[i][j] == 'O') {
        sum += height - i;
      }
    }
  }
  return sum;
}

int
main() {
  Board board;
  std::string line;
  while (std::getline(std::cin, line)) {
    board.push_back(line);
  }
  tilt(board);
  for (auto line : board) {
    std::cout << line << "\n";
  }
  Num sum = calcLoad(board);
  std::cout << sum << "\n";
  return 0;
}
