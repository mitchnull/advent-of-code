#include <ios>
#include <iostream>
#include <cctype>
#include <limits>
#include <deque>
#include <string>
#include <vector>
#include <bitset>
#include <algorithm>
#include <ranges>
#include <charconv>
#include <numeric>
#include <memory>
#include <unordered_set>
#include <string_view>
#include <variant>
#include <sstream>
#include <set>
#include <utility>

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

using Point = std::tuple<int, int, int>;

static const std::vector<Point> dirs = {
  { 1,  0,  0},
  {-1,  0,  0},
  { 0,  1,  0},
  { 0, -1,  0},
  { 0,  0,  1},
  { 0,  0, -1},
};

template<typename Tuple, typename Op, size_t... I>
static Tuple
tupleOp(const Tuple& a, const Tuple& b, Op &&op, std::index_sequence<I...>) {
  return {op(std::get<I>(a), std::get<I>(b))...};
}

static Point
operator+(const Point& a, const Point& b) {
  return tupleOp(a, b, std::plus<>(), std::make_index_sequence<std::tuple_size_v<Point>>{});
}

int
main() {
  Point p;
  char c;

  std::set<Point> cubes;

  while (std::cin >> std::get<0>(p) >> c >> std::get<1>(p) >> c >> std::get<2>(p)) {
    cubes.insert(p);
  }
  uint res = 0;
  for (const auto& cube: cubes) {
    for (const auto& d: dirs) {
      if (!cubes.contains(cube + d)) {
        ++res;
      }
    }
  }
  std::cout << res << "\n";
  return 0;
}
