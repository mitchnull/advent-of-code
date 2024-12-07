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
#include <unordered_set>

using uint8 = std::uint8_t;
using int8 = std::int8_t;
using uint32 = std::uint32_t;
using int32 = std::int32_t;
using uint64 = std::uint64_t;
using int64 = std::int64_t;

using uint = uint32;
using string = std::string;

constexpr const uint N = 6;
constexpr const uint M = 40;
constexpr const uint R = 20;

struct State {
  int x = 1;
  int nx = 1;
  uint cycleStart = 0;
  uint cycleEnd = 0;
};

static void
updateState(State& state, uint cycle) {
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

static int
signalStrength(State& state, uint cycle) {
  updateState(state, cycle);
  return cycle * state.x;
}

int
main() {
  State state;
  uint res = 0;
  for (uint i = 0, c = R; i < N; ++i, c += M) {
    res += signalStrength(state, c);
  }
  std::cout << res << "\n";
  return 0;
}
