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

struct Point {
  int x, y;
};

struct Segment {
  int a, b;

  friend bool
  operator<(const Segment& a, const Segment& b) {
    return a.a < b.a || (a.a == b.a && a.b < b.b);
  }
};

struct Sensor {
  Point p;
  uint r;
};

struct Line {
  Point a, b;
};

static auto
d(auto a, auto b) { 
  return std::abs(a - b);
};

static uint
r(int sx, int sy, int bx, int by) {
  return d(sx, bx) + d(sy, by);
}

static std::optional<Segment>
isect(const Sensor& sensor, int ty) {
  uint dy = d(sensor.p.y, ty);
  if (dy > sensor.r) {
    return {};
  }
  int dx = sensor.r - dy;
  std::cout << "dx=" << dx << ", x=" << sensor.p.x << "\n";
  return Segment{sensor.p.x - dx, sensor.p.x + dx};
}

int
main() {
  int sx, sy, bx, by;
  std::string s;
  char c;
  int maxY = 0;
  // Sensor at x=98246, y=1908027: closest beacon is at x=1076513, y=2000000
  std::vector<Sensor> sensors;
  std::vector<Point> beacons;
  while (std::cin >> s >> s >> c >> c >> sx >> c >> c >> c >> sy >> c >> s >> s >> s >> s >> c >> c >> bx >> c >> c >> c >> by) {
    maxY = std::max(maxY, sx);
    sensors.emplace_back(Sensor{{sx, sy}, r(sx, sy, bx, by)});
    beacons.emplace_back(Point{bx, by});
    std::cout << sensors.back().p.x << ", " << sensors.back().p.y << ", r=" << sensors.back().r << "\n";
  }

  int ty = maxY < 2000000 ? 10 : 2000000;
  std::set<int> beaconsAtTy;
  for (const auto& b: beacons) {
    if (b.y == ty) {
      beaconsAtTy.insert(b.x);
    }
  }
  std::vector<Segment> segments;
  
  for (const auto& s: sensors) {
    auto segment = isect(s, ty);
    if (segment.has_value()) {
      segments.push_back(*segment);
    }
  }
  std::sort(segments.begin(), segments.end());
  for (const auto& s: segments) {
    std::cout << "[" << s.a << ", " << s.b << "]\n";
  }
  segments.erase(std::remove_if(segments.begin() + 1, segments.end(), [prev = segments.begin()](const Segment& s) mutable {
        if (s.a <= prev->b) {
          prev->b = std::max(s.b, prev->b);
          return true;
        }
        ++prev;
        return false;
      }), segments.end());
  std::cout << "merged: \n";
  for (const auto& s: segments) {
    std::cout << "[" << s.a << ", " << s.b << "]\n";
  }
  uint res = std::accumulate(segments.begin(), segments.end(), 0U, [&](auto len, auto s) {
      return len + s.b - s.a + 1 - 
        std::count_if(beaconsAtTy.begin(), beaconsAtTy.end(), [&](int bx) { return s.a <= bx && bx <= s.b; });
    });
  std::cout << res << "\n";
}
