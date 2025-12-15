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

namespace ranges = std::ranges;
namespace views = ranges::views;

using Board = std::vector<std::string>;

struct Dir {
  int di, dj;
};

static void
tiltN(Board &board) {
  int height = board.size();
  int width = board.front().size();
  for (int i = 1; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      if (board[i][j] == 'O') {
        int k = i;
        while (--k >= 0 && board[k][j] == '.') {
          continue;
        }
        swap(board[i][j], board[++k][j]);
      }
    }
  }
}

static void
tiltS(Board &board) {
  int height = board.size();
  int width = board.front().size();
  for (int i = height - 2; i >= 0; --i) {
    for (int j = 0; j < width; ++j) {
      if (board[i][j] == 'O') {
        int k = i;
        while (++k < height && board[k][j] == '.') {
          continue;
        }
        swap(board[i][j], board[--k][j]);
      }
    }
  }
}

static void
tiltW(Board &board) {
  int height = board.size();
  int width = board.front().size();
  for (int j = 1; j < width; ++j) {
    for (int i = 0; i < height; ++i) {
      if (board[i][j] == 'O') {
        int k = j;
        while (--k >= 0 && board[i][k] == '.') {
          continue;
        }
        swap(board[i][j], board[i][++k]);
      }
    }
  }
}

static void
tiltE(Board &board) {
  int height = board.size();
  int width = board.front().size();
  for (int j = width - 2; j >= 0; --j) {
    for (int i = 0; i < height; ++i) {
      if (board[i][j] == 'O') {
        int k = j;
        while (++k < height && board[i][k] == '.') {
          continue;
        }
        swap(board[i][j], board[i][--k]);
      }
    }
  }
}

static Num
calcLoad(const Board &board) {
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

static void
cycle(Board &board) {
  tiltN(board);
  tiltW(board);
  tiltS(board);
  tiltE(board);
}

int
main() {
  Board board;
  std::string line;
  while (std::getline(std::cin, line)) {
    board.push_back(line);
  }

  std::vector<Board> boards;

  const int cycles = 1000000000;
  for (int i = 0; i < cycles; ++i) {
    auto it = std::find(boards.begin(), boards.end(), board);
    if (it != boards.end()) {
      int s = it - boards.begin();
      int m = i - s;
      int last = s + ((cycles - s) % m);
      // std::cout << "cycle: " << i << " -> " << s << " (" << m << "), last: " << last << "\n";
      board = boards[last];
      break;
    }
    boards.push_back(board);
    cycle(board);
  }
  // for (auto line : board) { std::cout << line << "\n"; }
  Num sum = calcLoad(board);
  std::cout << sum << "\n";
  return 0;
}
