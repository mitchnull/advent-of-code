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

using Num = int32;
constexpr const Num Min = std::numeric_limits<Num>::min();
constexpr const Num Max = std::numeric_limits<Num>::max();

constexpr const uint TURNS = 10;

struct Pos {
  Num x, y;

  friend auto
  operator<=>(const Pos& a, const Pos& b) = default;

  friend Pos
  operator+(Pos a, const Pos& b) {
    a.x += b.x;
    a.y += b.y;
    return a;
  }
};

struct ProposedPos : Pos {
  bool cancelled = false;
};

static constexpr const Pos Zero{};

const std::array<Pos, 4> Dirs = {
  Pos{0, -1},
  Pos{0, 1},
  Pos{-1, 0},
  Pos{1, 0},
};

namespace std {
  template<>
  struct hash<Pos> {
    std::size_t operator()(const Pos &p) const {
      return static_cast<size_t>(p.x) << 32 | p.y;
    }
  };
}

using Map = std::unordered_map<Pos, ProposedPos>;

static Pos
offset(Pos pos, const Pos& dir, int offset) {
  return dir.x == 0
    ? Pos{pos.x + offset, pos.y + dir.y}
    : Pos{pos.x + dir.x, pos.y + offset};
}

static bool
isEmpty(const Map& map, const Pos& pos, const Pos& dir) {
  return !map.contains(pos + dir)
    && !map.contains(offset(pos, dir, -1))
    && !map.contains(offset(pos, dir, +1));
}

static Pos
propose(const Map& map, const Pos& pos, uint turn) {
  bool isAllEmpty = true;
  Pos dir{};
  for (int i = 3; i >= 0; --i) {
    uint ci = (turn + i) % 4;
    if (isEmpty(map, pos, Dirs[ci])) {
      dir = Dirs[ci];
    } else {
      isAllEmpty = false;
    }
  }
  return isAllEmpty ? pos : pos + dir;
}

static Map
moveElves(const Map& map, uint turn) {
  Map res;
  for (const auto& [pos, ig]: map) {
    Pos next = propose(map, pos, turn);
    if (res.contains(next)) {
      ProposedPos& other = res[next];
      if (!other.cancelled) {
        other.cancelled = true;
        res[other] = {};
      }
      res[pos] = {};
    } else {
      res[next] = {pos, false};
    }
  }
  for (auto it = res.cbegin(), end = res.cend(); it != end; ) {
    it = it->second.cancelled ? res.erase(it) : ++it;
  }
  return res;
}

static std::pair<Pos, Pos>
bounds(const Map& map) {
  Pos tl{Max, Max}, br{Min, Min};
  for (const auto& [pos, ig]: map) {
    tl.x = std::min(tl.x, pos.x);
    tl.y = std::min(tl.y, pos.y);
    br.x = std::max(br.x, pos.x);
    br.y = std::max(br.y, pos.y);
  }
  return {tl, br};
}

static void
print(const Map& map) {
  auto [tl, br] = bounds(map);
  for (Num y = tl.y; y <= br.y; ++y) {
    for (Num x = tl.x; x <= br.x; ++x) {
      std::cout << (map.contains({x, y}) ? '#' : '.');
    }
    std::cout << "\n";
  }
}

int
main() {
  string line;
  Num y = 0;
  Map map;
  while (std::getline(std::cin, line) && !line.empty()) {
    for (Num x = 0, end = line.size(); x < end; ++x) {
      if (line[x] == '#') {
        Pos p{x, y};
        map[p] = {p, false};
      }
    }
    ++y;
  }
  print(map);
  for (uint turn = 0; turn < TURNS; ++turn) {
    map = moveElves(map, turn);
    std::cout << (turn + 1) << ":\n"; print(map);
  }
  auto [tl, br] = bounds(map);
  uint all = (br.x - tl.x + 1) * (br.y - tl.y + 1);
  uint res = all - map.size();
  std::cout << "(" << tl.x << ", " << tl.y << ") - (" << br.x << ", " << br.y << "): " << all << "\n";
  std::cout << res << "\n";
}
