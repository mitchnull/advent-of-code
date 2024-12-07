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

using Map = std::unordered_map<char, int>;

// Game 82: 8 green, 1 blue; 2 blue, 4 red; 7 green, 1 red, 4 blue; 2 green, 3 red, 2 blue; 3 red; 4 red, 8 green, 1 blue


static const Map Empty = {{'r', 0}, {'g', 0}, {'b', 0}};

static const int
check(const std::string& line) {
  std::stringstream ss;
  std::copy_if(line.begin(), line.end(), std::ostreambuf_iterator(ss), [](auto c) { return c != ',' && c != ';'; });
  std::string word;
  int game;

  Map minCounts = Empty;
  Map counts;
  int count;
  ss >> word >> game >> word;
  while (ss >> count >> word) {
    counts[word.front()] += count;
    if (word.back() != ',') {
      for (const auto& e : counts) {
        minCounts[e.first] = std::max(e.second, minCounts[e.first]);
      }
      counts = {};
    }
  }
  return std::ranges::fold_left(minCounts | std::views::transform([](auto e) { return e.second; }), 1, std::multiplies<>());
}

int
main() {
  int sum = 0;
  std::string line;
  while (std::getline(std::cin, line)) {
    sum += check(line);
  }
  std::cout << sum << "\n";
  return 0;
}
