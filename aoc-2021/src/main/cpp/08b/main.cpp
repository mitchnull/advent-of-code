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
#include <unordered_map>
#include <unordered_set>
#include <vector>

using uint8 = std::uint8_t;
using int8 = std::int8_t;
using uint32 = std::uint32_t;
using int32 = std::int32_t;
using uint64 = std::uint64_t;
using int64 = std::int64_t;

constexpr std::size_t N = 7;
using Segments = uint8;
constexpr Segments S0 = Segments{0};

using SSet = std::unordered_set<Segments>;

static Segments
segments(const std::string &str) {
  return std::accumulate(str.begin(), str.end(), S0, [](Segments s, char c) { return s | (1 << (c - 'a')); });
}

static Segments
ifUnique(const SSet &sset) {
  return sset.size() == 1 ? *sset.begin() : S0;
}

static bool
containsAll(Segments s, Segments c) {
  return (s & c) == c;
}

static int
decode(std::string line) {
  std::stringstream ss(line);
  std::string str;
  std::vector<SSet> bySize(N + 1);
  while (ss >> str && str != "|") {
    bySize[str.size()].insert(segments(str));
  }
  Segments cfSegments = ifUnique(bySize[2]);
  Segments acfSegments = ifUnique(bySize[3]);
  Segments bcdfSegments = ifUnique(bySize[4]);
  Segments allSegments = ifUnique(bySize[7]);
  Segments bdSegments = cfSegments ? bcdfSegments & ~cfSegments : S0;
  std::unordered_map<Segments, int> guessed;
  guessed[cfSegments] = 1;
  guessed[acfSegments] = 7;
  guessed[bcdfSegments] = 4;
  guessed[allSegments] = 8;
  for (auto s : bySize[5]) {
    if (cfSegments && containsAll(s, cfSegments)) {
      guessed[s] = 3;
    } else if (bdSegments) {
      if (containsAll(s, bdSegments)) {
        guessed[s] = 5;
      } else {
        guessed[s] = 2;
      }
    }
  }
  for (auto s : bySize[6]) {
    if (cfSegments) {
      if (containsAll(s, cfSegments)) {
        if (bcdfSegments) {
          if (containsAll(s, bcdfSegments)) {
            guessed[s] = 9;
          } else {
            guessed[s] = 0;
          }
        }
      } else {
        guessed[s] = 6;
      }
    } else if (bcdfSegments && containsAll(s, bcdfSegments)) {
      guessed[s] = 9;
    }
  }
  int res = 0;
  while (ss >> str) {
    res = res * 10 + guessed[segments(str)];
  }
  return res;
}

int
main() {
  int res = 0;
  for (std::string line; std::getline(std::cin, line);) {
    res += decode(line);
  }
  std::cout << res << "\n";
  return 0;
}
