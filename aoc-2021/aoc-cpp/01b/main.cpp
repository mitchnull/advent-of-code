#include <cctype>
#include <deque>
#include <iostream>
#include <limits>

using uint8 = std::uint8_t;
using int8 = std::int8_t;
using uint32 = std::uint32_t;
using int32 = std::int32_t;
using uint64 = std::uint64_t;
using int64 = std::int64_t;

using uint = uint32;

int
main() {
  uint last = std::numeric_limits<uint32>::max();
  uint c = 0;
  std::deque<uint> b;
  uint res = 0;
  uint d;
  while ((std::cin >> d)) {
    b.push_back(d);
    c += d;
    if (b.size() < 3) {
      continue;
    }
    if (b.size() > 3) {
      c -= b.front();
      b.pop_front();
    }
    // std::cout << c;
    if (c > last) {
     // std::cout << " (increase)";
      ++res;
    }
    // std::cout << "\n";
    last = c;
  }
  std::cout << res << "\n";
  return 0;
}
