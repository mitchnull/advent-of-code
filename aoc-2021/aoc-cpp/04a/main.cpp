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

using uint8 = std::uint8_t;
using int8 = std::int8_t;
using uint32 = std::uint32_t;
using int32 = std::int32_t;
using uint64 = std::uint64_t;
using int64 = std::int64_t;

using uint = uint32;
using Board = std::vector<std::pair<uint, bool>>;

static constexpr uint N = 5;

static bool
isWinner(const Board& b, uint row, uint col) {
  auto horiz = true;
  for (uint j = 0; j < N; ++j) {
    horiz = horiz & b[row * N + j].second;
  }
  if (horiz) {
    return true;
  }
  auto vert = true;
  for (uint i = 0; i < N; ++i) {
    vert = vert & b[i * N + col].second;
  }
  if (vert) {
    return true;
  }
  return false;
}

static uint
check(Board& board, uint n) {
  bool winner = false;
  for (uint i = 0; i < N; ++i) {
    for (uint j = 0; j < N; ++j) {
      auto& b = board[i * N + j];
      if (b.first == n) {
        b.second = true;
        winner = winner || isWinner(board, i, j);
      }
    }
  }
  if (!winner) {
    return 0;
  }
  uint sum = 0;
  for (const auto &b : board) {
    if (!b.second) {
      sum += b.first;
    }
  }
  return sum * n;
}

static bool
readBoard(std::vector<Board>& boards) {
  Board board;
  for (uint i = 0; i < N * N; ++i) {
    int n;
    if (!(std::cin >> n)) {
      return false;
    }
    board.emplace_back(n, false);
  }
  boards.push_back(board);
  return true;
}

int
main() {
  std::cin >> std::noskipws;
  uint n;
  char c = ',';
  std::vector<uint> draws;
  while (c == ',') {
    std::cin >> n >> c;
    draws.push_back(n);
  }
  std::cin >> std::skipws;

  std::vector<Board> boards;
  while (readBoard(boards)) {
    continue;
  }
  for (auto n : draws) {
    for (auto& board : boards) {
      auto res = check(board, n);
      if (res != 0) {
        std::cout << res << "\n";
        return 0;
      }
    }
  }
  std::cout << "0\n";
  return 0;
}
