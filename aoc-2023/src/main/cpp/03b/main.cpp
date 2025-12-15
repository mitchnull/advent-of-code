#include <algorithm>
#include <bitset>
#include <cctype>
#include <charconv>
#include <cstdint>
#include <deque>
#include <ios>
#include <iostream>
#include <limits>
#include <ranges>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using uint8 = std::uint8_t;
using int8 = std::int8_t;
using uint32 = std::uint32_t;
using int32 = std::int32_t;
using uint64 = std::uint64_t;
using int64 = std::int64_t;

using uint = uint32;

using Board = std::vector<std::string>;

struct Span {
  int i, j, k;
  friend auto operator<=>(const Span&, const Span&) = default;
};

using Spans = std::set<Span>;

static int
parseNum(const char* first, const char* last, int base = 10) {
  int res = 0;
  std::from_chars(first, last, res, base);
  return res;
}

static Span
findNum(const Board& board, int i, int j) {
  const auto& line = board[i];
  int w = line.size();
  int b = j;
  while (b >= 0 && std::isdigit(line[b])) {
    --b;
  }
  ++b;
  while (j < w && std::isdigit(line[j])) {
    ++j;
  }
  return {i, b, j};
}

static Spans
adjacentNumbers(const Board& board, int i, int j) {
  Spans spans;
  int h = board.size();
  int w = board[i].size();
  for (int ii = std::max(0, i -1); ii < std::min(i + 2, h); ++ii) {
    for (int jj = std::max(0, j - 1); jj < std::min(j + 2, w); ++jj) {
      if (std::isdigit(board[ii][jj])) {
        spans.insert(findNum(board, ii, jj));
      }
    }
  }
  return spans;
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
      if (board[i][j] == '*') {
        Spans spans = adjacentNumbers(board, i, j);
        if (spans.size() == 2) {
          int ratio = 1;
          for (const auto& span: spans) {
            const auto& line = board[span.i];
            ratio *= parseNum(line.data() + span.j, line.data() + span.k);
          }
          sum += ratio;
        }
      }
    }
  }
  std::cout << sum << "\n";
  return 0;
}
