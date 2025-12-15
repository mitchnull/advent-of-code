#include <cctype>
#include <deque>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

using uint8 = std::uint8_t;
using int8 = std::int8_t;
using uint32 = std::uint32_t;
using int32 = std::int32_t;
using uint64 = std::uint64_t;
using int64 = std::int64_t;

using uint = uint32;
using V = std::vector<uint>;

int
main() {
  std::string s;
  V ones = V();
  V zeros = V();
  
  while ((std::cin >> s)) {
    ones.resize(s.size());
    zeros.resize(s.size());
    for (auto i = 0U; i < s.length(); ++i) {
      switch (s[i]) {
        case '0':
          ++zeros[i];
          break;
        case '1':
          ++ones[i];
          break;
        default:
          std::cerr << "ERROR" << s << "\n";
          return -1;
      }
    }
  }
  int g = 0;
  int e = 0;
  for (auto i = 0U; i < ones.size(); ++i) {
    g <<= 1;
    e <<= 1;
    if (ones[i] > zeros[i]) {
      g |= 1;
    } else {
      e |= 1;
    }
  }
  std::cout << (g * e) << "\n";
  return 0;
}
