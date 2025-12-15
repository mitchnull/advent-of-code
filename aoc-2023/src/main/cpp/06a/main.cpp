#include <algorithm>
#include <bitset>
#include <cctype>
#include <charconv>
#include <cstdint>
#include <deque>
#include <ios>
#include <iostream>
#include <limits>
#include <map>
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

int
main() {
  std::string line, word;
  int n;

  std::getline(std::cin, line);
  std::stringstream timesLine(line);
  timesLine >> word;
  std::vector<int> times;
  while (timesLine >> n) {
    times.push_back(n);
  }

  std::getline(std::cin, line);
  std::stringstream lensLine(line);
  lensLine >> word;
  std::vector<int> lens;
  while (lensLine >> n) {
    lens.push_back(n);
  }

  int res = 1;
  for (int i = 0, size = times.size(); i < size; ++i) {
    int wins = 0;
    int time = times[i];
    for (int j = 1, mj = time - 1; j < mj; ++j) {
      int len = (time - j) * j;
      if (len > lens[i]) {
        ++wins;
      }
    }
    res *= wins;
  }
  std::cout << res << "\n";
  return 0;
}
