#include <algorithm>
#include <bitset>
#include <cctype>
#include <charconv>
#include <deque>
#include <ios>
#include <iostream>
#include <limits>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>

using uint8 = std::uint8_t;
using int8 = std::int8_t;
using uint32 = std::uint32_t;
using int32 = std::int32_t;
using uint64 = std::uint64_t;
using int64 = std::int64_t;

using uint = uint32;


int
main() {
  std::string line;
  uint curr = 0;
  std::vector<uint> elves;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      elves.push_back(curr);
      curr = 0;
      continue;
    }
    curr += std::stoi(line);
  }
  elves.push_back(curr);
  std::sort(elves.begin(), elves.end(), std::greater<>());
  std::cout << std::accumulate(elves.begin(), elves.size() < 3 ? elves.end() : elves.begin() + 3, 0) << "\n";
  return 0;
}
