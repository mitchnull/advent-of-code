#include <algorithm>
#include <bitset>
#include <cctype>
#include <charconv>
#include <cstdint>
#include <deque>
#include <ios>
#include <iostream>
#include <limits>
#include <numeric>
#include <ranges>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using uint8 = std::uint8_t;
using int8 = std::int8_t;
using uint32 = std::uint32_t;
using int32 = std::int32_t;
using uint64 = std::uint64_t;
using int64 = std::int64_t;

using uint = uint32;

using V = std::vector<int>;
using Counts = std::vector<int>;

// Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53

static void
inc(Counts &counts, std::size_t i, int increment = 1) {
  counts.resize(std::max(i + 1, counts.size()));
  counts[i] += increment;
}

int
main() {
  Counts counts;
  std::string line;
  int card = 0;
  while (std::getline(std::cin, line)) {
    inc(counts, card);
    int pos = line.find('|');
    std::stringstream first(line.substr(0, pos));
    std::stringstream second(line.substr(pos + 1));
    std::string word;
    int n;
    V winning;
    first >> word >> word;
    while (first >> n) {
      winning.push_back(n);
    }
    int numWinning = 0;
    while (second >> n) {
      if (std::find(winning.begin(), winning.end(), n) != winning.end()) {
        inc(counts, card + (++numWinning), counts[card]);
      }
    }
    ++card;
  }
  int64 sum = std::accumulate(counts.begin(), counts.begin() + card, 0L);
  std::cout << sum << "\n";
  return 0;
}
