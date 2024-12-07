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
  uint most = 0;
  uint curr = 0;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      most = std::max(curr, most);
      curr = 0;
      continue;
    }
    curr += std::stoi(line);
  }
  most = std::max(curr, most);
  std::cout << most << "\n";
  return 0;
}
