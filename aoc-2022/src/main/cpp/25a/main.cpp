#include <algorithm>
#include <bitset>
#include <cctype>
#include <charconv>
#include <deque>
#include <ios>
#include <iostream>
#include <limits>
#include <memory>
#include <numeric>
#include <ranges>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

using uint8 = std::uint8_t;
using int8 = std::int8_t;
using uint32 = std::uint32_t;
using int32 = std::int32_t;
using uint64 = std::uint64_t;
using int64 = std::int64_t;

using uint = uint32;
using string = std::string;
using namespace std::literals;
using size_t = std::size_t;

using Num = int64;

static Num
from5d(char c) {
  switch (c) {
    case '=':
      return -2;
    case '-':
      return -1;
  }
  return c - '0';
}

static char
to5d(Num d) {
  switch (d) {
    case -2:
      return '=';
    case -1:
      return '-';
  }
  return '0' + d;
}

static Num
from5(const string& str) {
  Num res = 0;
  for (auto it = str.cbegin(), end = str.cend(); it != end; ++it) {
    res *= 5;
    res += from5d(*it);
  }
  return res;
}

static string
to5(Num n) {
  string res;
  do {
    Num d = n % 5;
    n /= 5;
    if (d > 2) {
      d -= 5;
      ++n;
    }
    res += to5d(d);
  } while (n != 0);
  std::reverse(res.begin(), res.end());
  return res;
}

int
main() {
  Num res = 0;
  string line;
  while (std::getline(std::cin, line) && !line.empty()) {
    // std::cout << line << ": " << from5(line) << "\n";
    res += from5(line);
  }
  // std::cout << res << "\n";
  std::cout << to5(res) << "\n";
}
