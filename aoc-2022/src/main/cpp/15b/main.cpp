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

constexpr const long M = 4000000L;
constexpr const bool debug = false;

struct Point {
  int x, y;
};

struct Segment {
  int a, b;

  friend bool
  operator<(const Segment& a, const Segment& b) {
    return a.a < b.a || (a.a == b.a && a.b < b.b);
  }

  friend bool
  operator==(const Segment& a, const Segment& b) {
    return a.a == b.a && a.b == b.b;
  }
};

struct Stripe {
  Segment v;
  std::vector<Segment> segments;
};

struct Sensor {
  Point p;
  uint r;
};

struct Line {
  Point a, b;
};

static Point
rot(const Point& p) {
  return {p.x + p.y, -p.x + p.y};
}

static Point
tor(const Point& p) {
  return {(p.x - p.y) / 2, (p.x + p.y) / 2};
}

static auto
d(auto a, auto b) { 
  return std::abs(a - b);
};

static uint
r(int sx, int sy, int bx, int by) {
  return d(sx, bx) + d(sy, by);
}

static void
addStripe(std::vector<Stripe>& stripes, Segment v, Segment s) {
  auto it = stripes.begin();
  while (it != stripes.end() && it->v.b < v.a) {
    ++it;
  }
  if (it == stripes.end() || v.b < it->v.a) {
    // no intersection
    stripes.insert(it, {v, {s}});
    return;
  }
  if (v.a < it->v.a) {
    // intersect from below
    it = stripes.insert(it, {Segment{v.a, it->v.a - 1}, {s}}) + 1;
  } else if (it->v.a < v.a) {
    // intersect inside bottom
    it = stripes.insert(it, {Segment{it->v.a, v.a - 1}, it->segments}) + 1;
    it->v.a = v.a;
  }
  if (v.b < it->v.b) {
    // intersect inside top
    it = stripes.insert(it + 1, {Segment{v.b + 1, it->v.b}, it->segments}) - 1;
    it->v.b = v.b;
  }
  it->segments.push_back(s);
  if (v.b > it->v.b) {
    // intersect from above
    v.a = it->v.b + 1;
    addStripe(stripes, v, s);
  }
}

int
main() {
  int sx, sy, bx, by;
  std::string s;
  char c;
  std::vector<Stripe> stripes;
  // Sensor at x=98246, y=1908027: closest beacon is at x=1076513, y=2000000
  while (std::cin >> s >> s >> c >> c >> sx >> c >> c >> c >> sy >> c >> s >> s >> s >> s >> c >> c >> bx >> c >> c >> c >> by) {
    int dy = r(sx, sy, bx, by);
    Point p = rot({sx, sy - dy});
    Point q = rot({sx, sy + dy});
    Segment v = {std::min(p.y, q.y), std::max(p.y, q.y)};
    Segment s = {std::min(p.x, q.x), std::max(p.x, q.x)};
    addStripe(stripes, v, s);
  }

  for (auto& s: stripes) {
    std::sort(s.segments.begin(), s.segments.end());
    // std::cout << "orig:\n";
    // for (const auto& s: s.segments) {
    //   std::cout << "[" << s.a << ", " << s.b << "]\n";
    // }
    s.segments.erase(std::remove_if(s.segments.begin() + 1, s.segments.end(), [prev = s.segments.begin()](const Segment& s) mutable {
          if (s.a <= prev->b) {
            prev->b = std::max(s.b, prev->b);
            return true;
          }
          ++prev;
          return false;
        }), s.segments.end());
    if (debug) {
      std::cout << "[" << s.v.a << ", " << s.v.b << "]: ";
      for (const auto& ss: s.segments) {
        std::cout << "[" << ss.a << ", " << ss.b << "] ";
      }
      std::cout << "\n";
    }
    if (s.v.a == s.v.b) {
      auto prev = s.segments.begin();
      for (auto it = prev + 1; it != s.segments.end(); ++it) {
        for (int x = prev->b + 1; x < it->a; ++x) {
          Point r = tor(Point{x, s.v.a});
          if (0 <= r.x && r.x <= M && 0 <= r.y && r.y <= M) {
            std::cout << "FREQ(" << r.x << ", " << r.y << ") = " << M * r.x + r.y << "\n";
          }
        }
      }
    }
  }
  return 0;
}
