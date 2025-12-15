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

static int&
x(Point& p) {
  return std::get<0>(p);
}

static int&
y(Point& p) {
  return std::get<1>(p);
}

static int&
z(Point& p) {
  return std::get<2>(p);
}

static const int&
x(const Point& p) {
  return std::get<0>(p);
}

static const int&
y(const Point& p) {
  return std::get<1>(p);
}

static const int&
z(const Point& p) {
  return std::get<2>(p);
}

static constexpr const int MIN = std::numeric_limits<int>::min();
static constexpr const int MAX = std::numeric_limits<int>::max();

static bool
isInside(const Point& p, const Point& minP, const Point& maxP) {
  return
    x(minP) <= x(p) && x(p) <= x(maxP) &&
    y(minP) <= y(p) && y(p) <= y(maxP) &&
    z(minP) <= z(p) && z(p) <= z(maxP);
}

int
main() {
  Point p;
  char c;

  std::set<Point> cubes;
  std::set<Point> outer;

  Point minP{MAX, MAX, MAX}, maxP{MIN, MIN, MIN};

  while (std::cin >> std::get<0>(p) >> c >> std::get<1>(p) >> c >> std::get<2>(p)) {
    cubes.insert(p);
    x(minP) = std::min(x(minP), x(p));
    y(minP) = std::min(y(minP), y(p));
    z(minP) = std::min(z(minP), z(p));
    x(maxP) = std::max(x(maxP), x(p));
    y(maxP) = std::max(y(maxP), y(p));
    z(maxP) = std::max(z(maxP), z(p));
  }
  --x(minP); --y(minP); --z(minP);
  ++x(maxP); ++y(maxP); ++z(maxP);
  // std::cout << "min: (" << x(minP) << ", " << y(minP) << ", " << z(minP) << "), max: (" << x(maxP) << ", " << y(maxP) << ", " << z(maxP) << ")\n";

  std::deque<Point> queue{minP};

  while (!queue.empty()) {
    Point p = queue.front();
    queue.pop_front();
    outer.insert(p);
    for (const auto& d: dirs) {
      Point pp = p + d;
      if (!outer.contains(pp) && !cubes.contains(pp) && isInside(pp, minP, maxP)) {
        outer.insert(pp);
        queue.push_back(std::move(pp));
        // std::cout << "adding: (" << x(pp) << ", " << y(pp) << ", " << z(pp) << ")\n";
      }
    }
  }

  uint res = 0;
  for (const auto& cube: cubes) {
    for (const auto& d: dirs) {
      if (outer.contains(cube + d)) {
        ++res;
      }
    }
  }
  std::cout << res << "\n";
  return 0;
}
