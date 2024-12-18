#include <iostream>
#include <cstdint>
#include <string>
#include "../utils.h"

using Num = uint64_t;

using Plants = Grid<>;
using BoolMap = Grid<bool>;
using IntMap = Grid<int>;

static auto
pop_front(auto& queue) {
  auto res = queue.front();
  queue.pop_front();
  return res;
}

static Num
sides(const IntMap& ids, int id, Pos start, Dir outerDir, int outerBound, Dir innerDir, int innerBound) {
  Num res = 0;
  Pos outerStart = start;
  for (int o = 0; o < outerBound; ++o, outerStart += outerDir) {
    Pos p = outerStart;
    bool inside = false;
    for (int i = 0; i < innerBound; ++i, p += innerDir) {
      if (ids[p] == id && ids[p - outerDir] != id) {
        res += !inside;
        inside = true;
      } else {
        inside = false;
      }
    }
  }
  return res;
}

static Num
sides(const IntMap& ids, int id) {
  return
    sides(ids, id, {0, 0}, {0, 1}, ids.h(), {1, 0}, ids.w()) +
    sides(ids, id, {ids.w() - 1, 0}, {-1, 0}, ids.w(), {0, 1}, ids.h()) +
    sides(ids, id, {ids.w() - 1, ids.h() - 1}, {0, -1}, ids.h(), {-1, 0}, ids.w()) +
    sides(ids, id, {0, ids.h() - 1}, {1, 0}, ids.h(), {0, -1}, ids.w());
}

static std::pair<Num, Num>
solve(const Plants& plants) {
  auto visited = BoolMap(plants.w(), plants.h());
  auto ids = IntMap(plants.w(), plants.h());
  auto areas = std::vector<Num>{0};

  Num res1 = 0;
  int id = 0;
  for (int x = 0; x < plants.w(); ++x) {
    for (int y = 0; y < plants.h(); ++y) {
      Pos p0{x, y};
      if (visited[p0]) {
        continue;
      }
      char plant = plants[p0];
      Num area = 0, perimiter = 0;
      std::deque<Pos> queue{p0};
      ++id;
      while (!queue.empty()) {
        auto p = pop_front(queue);
        if (visited[p]) {
          continue;
        }
        visited[p] = true;
        ids[p] = id;
        ++area;
        for (auto d: DIRS) {
          auto pp = p + d;
          if (plants[pp] != plant) {
            ++perimiter;
          } else if (!visited[pp]) {
            queue.push_back(pp);
          }
        }
      }
      areas.push_back(area);
      res1 += area * perimiter;
    }
  }

  Num res2 = 0;
  for (int i = 1; i <= id; ++i) {
    res2 += areas[i] * sides(ids, i);
  }
  return {res1, res2};
}

/* ------------------------------------------------------------------------ */

int
main() {
  std::vector<std::string> lines;
  std::string line;
  while (std::cin >> line) {
    lines.emplace_back(line);
  }
  const auto plants = Plants(lines);
  auto [res1, res2] = solve(plants);
  std::cout << "1: " << res1 << "\n";
  std::cout << "2: " << res2 << "\n";

  return 0;
}
