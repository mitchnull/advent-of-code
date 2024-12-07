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

using V = std::vector<int>;

// Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53

int
main() {
  int64 sum = 0;
  std::string line;
  while (std::getline(std::cin, line)) {
    int pos = line.find('|');
    std::stringstream first(line.substr(0, pos));
    std::stringstream second(line.substr(pos + 1));
    std::string word;
    int n;
    V winning;
    first >> word  >> word;
    while (first >> n) {
      winning.push_back(n);
    }
    int numWinning = 0;
    while (second >> n) {
      if (std::find(winning.begin(), winning.end(), n) != winning.end()) {
        ++numWinning;
      }
    }
    if (numWinning > 0) {
      sum += 1UL << (numWinning - 1);
    }
  }
  std::cout << sum << "\n";
  return 0;
}
