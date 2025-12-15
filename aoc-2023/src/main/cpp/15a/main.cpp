#include <algorithm>
#include <bitset>
#include <cctype>
#include <cstdint>
#include <deque>
#include <ios>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <ranges>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

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
