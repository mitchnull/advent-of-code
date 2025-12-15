#include <algorithm>
#include <bitset>
#include <cctype>
#include <charconv>
#include <cstdint>
#include <deque>
#include <ios>
#include <iostream>
#include <limits>
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

using Map = std::unordered_map<char, int>;

// Game 82: 8 green, 1 blue; 2 blue, 4 red; 7 green, 1 red, 4 blue; 2 green, 3 red, 2 blue; 3 red; 4 red, 8 green, 1 blue


static const Map maxCubes = {{'r', 12}, {'g', 13}, {'b', 14}};

static const int
check(const std::string& line) {
  std::stringstream ss;
  std::copy_if(line.begin(), line.end(), std::ostreambuf_iterator(ss), [](auto c) { return c != ',' && c != ';'; });
  std::string word;
  int game;

  Map counts;
  int count;
  ss >> word >> game >> word;
  while (ss >> count >> word) {
    counts[word.front()] += count;
    if (word.back() != ',') {
      for (const auto& e : counts) {
        auto it = maxCubes.find(e.first);
        if (it == maxCubes.end() || e.second > it->second) {
          return 0;
        }
      }
      counts = {};
    }
  }
  return game;
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
