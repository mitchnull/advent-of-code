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
#include <unordered_set>
#include <vector>

using uint8 = std::uint8_t;
using int8 = std::int8_t;
using uint32 = std::uint32_t;
using int32 = std::int32_t;
using uint64 = std::uint64_t;
using int64 = std::int64_t;

using uint = uint32;
using string = std::string;

constexpr const uint M = 40;

struct State {
  int x = 1;
  int nx = 1;
  uint cycleStart = 0;
  uint cycleEnd = 0;
};

static void
updateState(State &state, uint cycle) {
  while (state.cycleEnd < cycle) {
    state.x = state.nx;
    state.cycleStart = state.cycleEnd + 1;
    string cmd;
    std::cin >> cmd;
    if (cmd == "addx") {
      int v;
      std::cin >> v;
      state.nx = state.x + v;
      state.cycleEnd = state.cycleStart + 1;
    } else {
      state.nx = state.x;
      state.cycleEnd = state.cycleStart;
    }
  }
}

static void
plot(State &state, uint cycle) {
  updateState(state, cycle);
  int px = (cycle - 1) % M;
  std::cout << ((state.x - 1 <= px && px <= state.x + 1) ? '#' : '.');
  if (px == M - 1) {
    std::cout << "\n";
  }
}

int
main() {
  State state;
  for (uint c = 1; (std::cin); ++c) {
    plot(state, c);
  }
  return 0;
}
