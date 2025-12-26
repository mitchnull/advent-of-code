#include "../utils.h"
#include <algorithm>
#include <iterator>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

using Pos3d = std::array<int, 3>;
using Pos3dSet = std::unordered_set<Pos3d>;
using Beacons = std::vector<Pos3d>;

static Pos3d
operator+(const Pos3d &a, const Pos3d &b) {
  return {a[0] + b[0], a[1] + b[1], a[2] + b[2]};
}

static Pos3d
operator-(const Pos3d &a, const Pos3d &b) {
  return {a[0] - b[0], a[1] - b[1], a[2] - b[2]};
}

template <>
struct std::hash<Pos3d> {
  std::size_t operator()(const Pos3d &p) const { return hashCombine(p[0], hashCombine(p[1], p[2])); }
};

struct Sensor {
  Pos3d p;
  Beacons beacons;
};
using Sensors = std::vector<Sensor>;

struct Facing {
  std::array<int, 3> perm = {0, 1, 2};
  int flip = 0;
};

static bool
next(Facing &f) {
  return (f.flip = ((f.flip + 1) % 8)) != 0 || std::next_permutation(f.perm.begin(), f.perm.end());
}

static Pos3d
spin(const Pos3d &p, const Facing &f) {
  Pos3d res;
  for (int i = 0; i < 3; ++i) {
    res[i] = (f.flip & (1 << i)) ? -p[f.perm[i]] : p[f.perm[i]];
  }
  return res;
}

static Sensor
spin(Sensor s, const Facing &f) {
  std::transform(s.beacons.begin(), s.beacons.end(), s.beacons.begin(), [&](auto p) { return spin(p, f); });
  return s;
}

static std::pair<bool, Pos3d>
findPlace(const Sensor &s, Pos3dSet &all) {
  Facing f;
  do {
    Sensor ss = spin(s, f);
    std::unordered_map<Pos3d, int> diffs;
    for (const auto &sb : ss.beacons) {
      for (const auto &ab : all) {
        auto d = ab - sb;
        if (++diffs[d] >= 12) {
          std::transform(ss.beacons.begin(), ss.beacons.end(), std::inserter(all, all.end()), [&](const auto &b) {
            return b + d;
          });
          return {true, d};
        }
      }
    }
  } while (next(f));
  return {false, {}};
}

static bool
findPlace(Sensors &sensors, auto i, Pos3dSet &all) {
  for (auto j = i; j < sensors.end(); ++j) {
    auto [found, p] = findPlace(*j, all);
    if (found) {
      if (i != j) {
        std::swap(*i, *j);
      }
      i->p = p;
      return true;
    }
  }
  return false;
}

static int
md(const Pos3d &a, const Pos3d &b) {
  int res = 0;
  for (int i = 0; i < 3; ++i) {
    res += std::abs(a[i] - b[i]);
  }
  return res;
}

static std::pair<int, int>
solve(Sensors sensors) {
  sensors.front().p = {};
  Pos3dSet all{sensors.front().beacons.begin(), sensors.front().beacons.end()};

  for (auto i = sensors.begin() + 1; i < sensors.end(); ++i) {
    if (!findPlace(sensors, i, all)) {
      return {-1, -1};
    }
  }
  int res2 = 0;
  for (auto i = sensors.begin(), end = sensors.end(); i != end; ++i) {
    for (auto j = i + 1; j != end; ++j) {
      res2 = std::max(res2, md(i->p, j->p));
    }
  }
  return {all.size(), res2};
}

/* ------------------------------------------------------------------------ */

int
main() {
  std::string line;
  Sensors sensors;
  while (std::getline(std::cin, line)) {
    Sensor s;
    while (std::getline(std::cin, line) && !line.empty()) {
      char ch;
      int x, y, z;
      std::istringstream ss{line};
      ss >> x >> ch >> y >> ch >> z;
      s.beacons.push_back({x, y, z});
    }
    sensors.push_back(std::move(s));
  }
  auto [res1, res2] = solve(sensors);
  std::println("1: {}", res1);
  std::println("2: {}", res2);
  return 0;
}
