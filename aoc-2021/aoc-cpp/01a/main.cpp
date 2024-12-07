#include <iostream>
#include <cctype>
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
  uint res = 0;
  uint d;
  while ((std::cin >> d)) {
    if (d > last) {
      ++res;
    }
    last = d;
  }
  std::cout << res << "\n";
  return 0;
}
