#include <ios>
#include <iostream>
#include <cctype>
#include <cstdint>
#include <limits>
#include <deque>
#include <string>
#include <vector>
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

using Board = std::vector<std::string>;

static int
parseNum(const char* first, const char* last, int base = 10) {
  int res = 0;
  std::from_chars(first, last, res, base);
  return res;
}

static bool
isSymbol(char ch) {
  return ch != '.' && !std::isdigit(ch);
}

static bool
isAdjacent(const Board& board, int i, int j, int k) {
  const std::string& line = board[i];
  int h = board.size();
  int w = line.size();
  if ((j > 0 && isSymbol(line[j -1])) || (k < w && isSymbol(line[k]))) {
    return true;
  }
  if (i > 0) {
    const std::string& line = board[i - 1];
    for (int jj = std::max(j - 1, 0); jj < std::min(k + 1, w); ++jj) {
      if (isSymbol(line[jj])) {
        return true;
      }
    }
  }
  if (i < h - 1) {
    const std::string& line = board[i + 1];
    for (int jj = std::max(j - 1, 0); jj < std::min(k + 1, w); ++jj) {
      if (isSymbol(line[jj])) {
        return true;
      }
    }
  }
  return false;
}

int
main() {
  int sum = 0;
  Board board;
  std::string line;
  while (std::getline(std::cin, line)) {
    board.push_back(line);
  }
  int h = board.size();
  int w = board.front().size();
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      if (std::isdigit(board[i][j])) {
        int k = j + 1;
        while (k < w && std::isdigit(board[i][k])) {
          ++k;
        }
        if (isAdjacent(board, i, j, k)) {
          sum += parseNum(board[i].data() + j, board[i].data() + k);
        }
        j = k;
      }
    }
  }
  std::cout << sum << "\n";
  return 0;
}
