#include <iostream>
#include <cctype>
#include <limits>
#include <deque>
#include <string>

using uint8 = std::uint8_t;
using int8 = std::int8_t;
using uint32 = std::uint32_t;
using int32 = std::int32_t;
using uint64 = std::uint64_t;
using int64 = std::int64_t;

using uint = uint32;

int
main() {
  std::string dir;
  uint c;
  uint x = 0;
  uint y = 0;
  while ((std::cin >> dir) && (std::cin >> c)) {
    if (dir == "forward") {
      x += c;
    } else if (dir == "up") {
      y -= c;
    } else if (dir == "down") {
      y += c;
    } else {
      std::cerr << "ERROR: " << dir << "\n";
      return -1;
    }
  }
  std::cout << (x * y) << "\n";
  return 0;
}
