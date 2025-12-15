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
#include <sstream>
#include <string>
#include <vector>

using uint8 = std::uint8_t;
using int8 = std::int8_t;
using uint32 = std::uint32_t;
using int32 = std::int32_t;
using uint64 = std::uint64_t;
using int64 = std::int64_t;

static int
countUnique(std::string line) {
  std::stringstream ss(line);
  std::string str;
  while (ss >> str && str != "|") {
    continue;
  }
  int res = 0;
  while (ss >> str) {
    switch (str.size()) {
      case 2:
      case 3:
      case 4:
      case 7: ++res;
    }
  }
  return res;
}

int
main() {
  int res = 0;
  for (std::string line; std::getline(std::cin, line);) {
    res += countUnique(line);
  }
  std::cout << res << "\n";
  return 0;
}
