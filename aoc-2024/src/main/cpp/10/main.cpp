#include <iostream>
#include <cstdint>
#include <string>
#include <functional>
#include <set>

using uint = std::uint32_t;

template<typename T>
class Map {
  using ValueType = std::conditional<std::is_same<T, bool>::value, char, T>::type;
  int w_, h_;
  std::vector<ValueType> data_;
  ValueType off_;
public:
  template<typename Tr = std::identity>
  Map(std::vector<std::string> lines, Tr tr = {}, T off = 0) : w_(lines[0].size()), h_(lines.size()), data_{}, off_(off) {
    data_.reserve(w_ * h_);
    for (const auto& line: lines) {
      for (char c: line) {
        data_.push_back(tr(c));
      }
    }
  }

  Map(int w, int h, T init = {}, T off = {}) : w_(w), h_(h), data_(w * h, init), off_(off) {}

  ValueType operator[](int x, int y) const {
    if (0 <= x && x < w_ && 0 <= y && y < h_) {
      return data_[y * w_ + x];
    }
    return 0;
  }
  ValueType& operator[](int x, int y) {
    static ValueType off;
    if (0 <= x && x < w_ && 0 <= y && y < h_) {
      return data_[y * w_ + x];
    }
    return off = off_;
  }
  int w() const { return w_; }
  int h() const { return h_; };
};

static auto
pop_front(auto& container) {
  auto res = container.front();
  container.pop_front();
  return res;
}

struct Dir {
  int dx, dy;
};

struct Pos {
  int x, y;

  Pos& operator+=(Dir d) {
    x += d.dx;
    y += d.dy;
    return *this;
  }

  Pos operator+(Dir d) {
    return Pos(*this) += d;
  }

  inline auto friend operator<=>(const Pos& a, const Pos& b) = default;
};

const auto DIRS = std::vector<Dir> {
  { 0, -1},
  { 1, 0},
  { 0, 1},
  { -1, 0},
};

using IntMap = Map<int>;
using BoolMap = Map<bool>;
using PosSetMap = Map<std::set<Pos>>;
using Num = int;

// ------------------------------------------------------------------------ //

static Num
score1(const IntMap& map, Pos start) {
  auto queue = std::deque<Pos>{start};
  auto visited = BoolMap(map.w(), map.h());

  Num res = 0;
  while (!queue.empty()) {
    Pos p = pop_front(queue);
    if (visited[p.x, p.y]) {
      continue;
    }
    int h = map[p.x, p.y];
    if (h == 9) {
      ++res;
      visited[p.x, p.y] = true;
      continue;
    }
    visited[p.x, p.y] = true;
    for (auto d: DIRS) {
      Pos pp = p + d;
      int hh = map[pp.x, pp.y];
      if (hh == h + 1 && !visited[pp.x, pp.y]) {
        queue.push_back(pp);
      }
    }
  }
  return res;
}

static Num
solve1(const IntMap& map) {
  Num res = 0;
  for (int x = 0; x < map.w(); ++x) {
    for (int y = 0; y < map.h(); ++y) {
      if (map[x, y] == 0) {
        res += score1(map, {x, y});
      }
    }
  }
  return res;
}

// ------------------------------------------------------------------------ //

static Num
solve2(const IntMap& map) {
  auto ways = IntMap(map.w(), map.h());
  for (int x = 0; x < map.w(); ++x) {
    for (int y = 0; y < map.h(); ++y) {
      if (map[x, y] == 0) {
        ways[x, y] = 1;
      }
    }
  }
  Num res = 0;
  for (int h = 0; h < 9; ++h) {
    res = 0;
    for (int x = 0; x < map.w(); ++x) {
      for (int y = 0; y < map.h(); ++y) {
        if (map[x, y] == h) {
          for (auto d: DIRS) {
            auto xx = x + d.dx;
            auto yy = y + d.dy;
            int hh = map[xx, yy];
            if (hh == h + 1) {
              ways[xx, yy] += ways[x, y];
              res += ways[x, y];
            }
          }
        }
      }
    }
  }
  return res;
}

// ------------------------------------------------------------------------ //

int
main() {
  std::vector<std::string> lines;
  std::string line;
  while (std::cin >> line) {
    lines.emplace_back(line);
  }
  const auto map = IntMap(lines, [](auto c) { return c - '0'; });

  std::cout << "1: " << solve1(map) << "\n";
  std::cout << "2: " << solve2(map) << "\n";

  return 0;
}
