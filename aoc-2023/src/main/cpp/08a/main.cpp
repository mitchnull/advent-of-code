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

namespace ranges = std::ranges;
namespace views = ranges::views;

using Node = std::unordered_map<char, std::string>;
using Nodes = std::unordered_map<std::string, Node>;

static char
ignore(char c) {
  if (!isspace(c) && !isalpha(c)) {
    return ' ';
  }
  return c;
}

int
main() {
  std::string line, steps;
  std::getline(std::cin, steps);
  Nodes nodes;
  while (std::getline(std::cin, line)) {
    std::transform(line.begin(), line.end(), line.begin(), ignore);
    std::string start, left, right;
    std::stringstream ss(line);
    if (ss >> start >> left >> right) {
      nodes[start] = {{'L', left}, {'R', right}};
    }
  }

  int count = 0;
  std::string node = "AAA";
  while (node != "ZZZ") {
    char step = steps[count++ % steps.size()];
    node = nodes[node][step];
  }
  std::cout << count << "\n";
  return 0;
}
