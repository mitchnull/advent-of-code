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

using uint8 = std::uint8_t;
using int8 = std::int8_t;
using uint32 = std::uint32_t;
using int32 = std::int32_t;
using uint64 = std::uint64_t;
using int64 = std::int64_t;

using uint = uint32;
static constexpr uint N = 7;

using Ages = std::vector<uint64>;

static uint
idx(uint i, uint offset) {
  return (offset + i) % N;
}

static void
tick(Ages &ages, uint tick) {
  uint zi = idx(0, tick);
  auto hatching = ages[zi];
  ages[zi] += ages[N];
  ages[N] = ages[N + 1];
  ages[N + 1] = hatching;
}

int
main() {
  std::cin >> std::noskipws;
  uint n;
  char c = ',';
  Ages ages = Ages(N + 2, 0);
  while (c == ',') {
    std::cin >> n >> c;
    ++ages[n];
  }
  for (uint i = 0; i < 256; ++i) {
    tick(ages, i);
  }

  std::cout << std::accumulate(ages.begin(), ages.end(), 0L) << "\n";
  return 0;
}
