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
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_set>
#include <variant>
#include <vector>

using uint8 = std::uint8_t;
using int8 = std::int8_t;
using uint32 = std::uint32_t;
using int32 = std::int32_t;
using uint64 = std::uint64_t;
using int64 = std::int64_t;

using uint = uint32;
using string = std::string;
using namespace std::literals;
using size_t = std::size_t;

constexpr const uint N = 2022;
constexpr const uint VD = 3;

using Bits = std::bitset<9>;
constexpr const Bits wall = Bits(0b100000001);
constexpr const Bits mask = Bits(0b011111110);
constexpr const Bits bottom = Bits(0b111111111);

using Rock = std::vector<Bits>;
using Board = std::vector<Bits>;

const std::vector<Rock> rocks{
    {
        0b000111100,
    },
    {
        0b000010000,
        0b000111000,
        0b000010000,
    },
    {
        0b000001000,
        0b000001000,
        0b000111000,
    },
    {
        0b000100000,
        0b000100000,
        0b000100000,
        0b000100000,
    },
    {
        0b000110000,
        0b000110000,
    },
};

static void
grow(Board &board, const Rock &r) {
  for (uint y = board.size(); y-- > 0;) {
    if ((board[y] & mask).any()) {
      uint ns = y + VD + r.size() + 1;
      while (board.size() < ns) {
        board.push_back(wall);
      }
      if (board.size() > ns) {
        board.erase(board.begin() + ns, board.end());
      }
      return;
    }
  }
}

static bool
coll(const Board &board, const Rock &rock, int x, uint y) {
  for (uint j = 0; j < rock.size(); ++j) {
    Bits rb = (x < 0) ? (rock[j] << -x) : (rock[j] >> x);
    if ((board[y - j] & rb).any()) {
      return true;
    }
  }
  return false;
}

static void
merge(Board &board, const Rock &rock, int x, uint y) {
  for (uint j = 0; j < rock.size(); ++j) {
    Bits rb = (x < 0) ? (rock[j] << -x) : (rock[j] >> x);
    board[y - j] |= rb;
  }
}

static void
print(const Board &board, const char *title) {
  std::cout << title << ":\n";
  for (uint y = board.size(); y-- > 0;) {
    const Bits &b = board[y];
    for (uint i = b.size(); i-- > 0;) {
      std::cout << ((b[i]) ? '#' : '.');
    }
    std::cout << "\n";
  }
}

int
main() {
  string air;
  std::cin >> air;
  Board board{bottom};
  const uint rmod = rocks.size();
  const uint amod = air.size();
  uint ai = 0;
  for (uint c = 0; c < N; ++c) {
    const Rock &r = rocks[c % rmod];
    grow(board, r);
    uint y = board.size() - 1;
    int x = 0;
    while (true) {
      switch (air[ai]) {
        case '<':
          if (!coll(board, r, x - 1, y)) {
            --x;
          }
          break;
        case '>':
          if (!coll(board, r, x + 1, y)) {
            ++x;
          }
          break;
      }
      ai = (ai + 1) % amod;
      if (coll(board, r, x, y - 1)) {
        merge(board, r, x, y);
        break;
      }
      --y;
    }
  }
  for (uint y = board.size() - 1; y > 0; --y) {
    if ((board[y] & mask).any()) {
      std::cout << y << "\n";
      break;
    }
  }
  return 0;
}
