#include "../utils.h"
#include <algorithm>
#include <iterator>
#include <optional>
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

using Sensors = std::vector<Beacons>;

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

static Beacons
spin(Beacons s, const Facing &f) {
  std::transform(s.begin(), s.end(), s.begin(), [&](auto p) { return spin(p, f); });
  return s;
}

static std::optional<Pos3d>
findPlace(const Beacons &s, Pos3dSet &all) {
  Facing f;
  do {
    Beacons ss = spin(s, f);
    std::unordered_map<Pos3d, int> diffs;
    for (const auto &sb : ss) {
      for (const auto &ab : all) {
        auto d = ab - sb;
        if (++diffs[d] >= 12) {
          std::transform(ss.begin(), ss.end(), std::inserter(all, all.end()), [&](const auto &b) { return b + d; });
          return d;
        }
      }
    }
  } while (next(f));
  return {};
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
  Beacons sps{{}};
  Pos3dSet all{sensors.front().begin(), sensors.front().end()};
  sensors.erase(sensors.begin());
  while (!sensors.empty()) {
    std::erase_if(sensors, [&](const auto &s) {
      auto p = findPlace(s, all);
      if (p) {
        sps.push_back(*p);
      }
      return p;
    });
  }
  int res2 = 0;
  for (auto i = sps.begin(), end = sps.end(); i != end; ++i) {
    for (auto j = i + 1; j != end; ++j) {
      res2 = std::max(res2, md(*i, *j));
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
    Beacons s;
    while (std::getline(std::cin, line) && !line.empty()) {
      char ch;
      int x, y, z;
      std::istringstream ss{line};
      ss >> x >> ch >> y >> ch >> z;
      s.push_back({x, y, z});
    }
    sensors.push_back(std::move(s));
  }
  auto [res1, res2] = solve(sensors);
  std::println("1: {}", res1);
  std::println("2: {}", res2);
  return 0;
}
