#include "../utils.h"
#include <algorithm>
#include <sstream>

using Pos3d = std::array<int, 3>;
using Beacons = std::vector<Pos3d>;

static Pos3d
operator+(const Pos3d &a, const Pos3d &b) {
  return {a[0] + b[0], a[1] + b[1], a[2] + b[2]};
}

static Pos3d
operator-(const Pos3d &a, const Pos3d &b) {
  return {a[0] - b[0], a[1] - b[1], a[2] - b[2]};
}

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
  ranges::sort(s.beacons);
  return s;
}

static Beacons
combine(const Sensor &s1, const Sensor &s2) {
  Beacons res;
  res.reserve(s1.beacons.size() + s2.beacons.size());
  auto b1 = s1.beacons | views::transform([&](const auto &b) { return s1.p + b; });
  auto b2 = s2.beacons | views::transform([&](const auto &b) { return s2.p + b; });
  ranges::set_union(b1, b2, std::back_inserter(res), {});
  return res;
}

static bool
checkOverlap(const Sensor &os, Sensor &s) {
  const auto overlapSize = os.beacons.size() + s.beacons.size() - 12;
  for (const auto &i : os.beacons) {
    for (const auto &j : s.beacons) {
      s.p = i + os.p - j;
      auto combined = combine(os, s);
      if (combined.size() <= overlapSize) {
        return true;
      }
    }
  }
  return false;
};

static std::pair<bool, Sensor>
findPlace(auto b, auto e, const Sensor &s) {
  for (auto it = b; it < e; ++it) {
    Facing f;
    do {
      Sensor ss = spin(s, f);
      if (checkOverlap(*it, ss)) {
        return {true, ss};
      }
    } while (next(f));
  }
  return {false, s};
}

static bool
findPlace(Sensors &sensors, auto i) {
  for (auto j = i; j < sensors.end(); ++j) {
    auto [found, s] = findPlace(sensors.begin(), i, *j);
    if (found) {
      if (i != j) {
        *j = std::move(*i);
      }
      *i = std::move(s);
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
  ranges::sort(sensors.front().beacons);
  for (auto i = sensors.begin() + 1; i < sensors.end(); ++i) {
    if (!findPlace(sensors, i)) {
      return {-1, -1};
    }
  }
  Sensor all{};
  for (const auto &s : sensors) {
    all.beacons = combine(all, s);
  }
  int res2 = 0;
  for (auto i = sensors.begin(), end = sensors.end(); i != end; ++i) {
    for (auto j = i + 1; j != end; ++j) {
      res2 = std::max(res2, md(i->p, j->p));
    }
  }
  return {all.beacons.size(), res2};
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
