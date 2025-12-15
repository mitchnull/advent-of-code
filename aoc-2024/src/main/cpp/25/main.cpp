#include <iostream>
#include <ranges>
#include <string>
#include <vector>

using Num = std::int64_t;
using Heights = std::vector<int>;
using HeightsList = std::vector<Heights>;

static bool
fits(Heights key, Heights lock, int hmax) {
  for (auto [k, l] : std::views::zip(key, lock)) {
    if (k + l > hmax) {
      return false;
    }
  }
  return true;
}

static Num
solve1(const HeightsList &keys, const HeightsList &locks, int hmax) {
  Num res = 0;
  for (auto &k : keys) {
    for (auto &l : locks) {
      res += fits(l, k, hmax);
    }
  }
  return res;
}

int
main() {
  HeightsList keys;
  HeightsList locks;

  std::string line;
  Heights h;
  bool isLock;
  int hmax = 0;
  int cmax = 0;
  while (std::getline(std::cin, line)) {
    ++cmax;
    if (line.empty()) {
      if (isLock) {
        locks.push_back(h);
      } else {
        keys.push_back(h);
      }
      h.clear();
    }
    if (h.empty()) {
      isLock = line[0] == '#';
      h.resize(line.size(), -1);
      hmax = std::max(hmax, cmax);
      cmax = 0;
    }
    for (int i = 0; i < line.size(); ++i) {
      h[i] += line[i] == '#';
    }
  }
  if (!h.empty()) {
    if (isLock) {
      locks.push_back(h);
    } else {
      keys.push_back(h);
    }
  }
  hmax -= 2;

  std::cout << "1: " << solve1(keys, locks, hmax) << std::endl;
}
