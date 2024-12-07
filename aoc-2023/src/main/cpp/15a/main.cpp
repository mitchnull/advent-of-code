#include <ios>
#include <iostream>
#include <cctype>
#include <cstdint>
#include <limits>
#include <deque>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <bitset>
#include <algorithm>
#include <ranges>
#include <sstream>
#include <numeric>

using Num = std::uint64_t;
using std::swap;

int
main() {
  char ch;
  uint8_t hash = 0;
  Num sum = 0;
  while (std::cin >> ch) {
    if (ch == ',') {
      sum += hash;
      hash = 0;
      continue;
    }
    hash += (ch & 0xff);
    hash *= 17;
  }
  sum += hash;
  std::cout << sum << "\n";
  return 0;
}
