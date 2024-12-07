#include <ios>
#include <iostream>
#include <cctype>
#include <cstdint>
#include <limits>
#include <deque>
#include <string>
#include <vector>
#include <map>
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

using Num = int64;

static Num
readNum(std::istream& in) {
  std::string line;
  in >> line;
  std::getline(in, line);
  line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
  return std::stol(line);
}

int
main() {
  Num time = readNum(std::cin);
  Num minLen = readNum(std::cin);

  int wins = 0;
  for (int j = 1, mj = time - 1; j < mj; ++j) {
    Num len = (time - j) * j;
    if (len > minLen) {
      ++wins;
    }
  }
  std::cout << wins << "\n";
  return 0;
}
