#include <iostream>
#include <cstdint>
#include <string>
#include <functional>
#include <set>
#include <ranges>

namespace views = std::views;
using uint = std::uint32_t;

template<typename T>
class Map {
  using value_type = std::conditional<std::is_same<T, bool>::value, char, T>::type;
  int w_, h_;
  std::vector<value_type> data_;
  value_type off_;

  template <typename V>
  struct Iter {
    int x, y;
    V v;
  };
  template <typename M>
  static auto iter_(M& map) {
    return views::iota(0UZ, map.data_.size()) | views::transform([&map](int i) { return Iter<decltype(map[0,0])>{i % map.w_, i / map.w_, map.data_[i]}; });
  }
public:
  template<typename Tr = std::identity>
  Map(std::vector<std::string> lines, Tr tr = {}, value_type off = {}) : w_(lines[0].size()), h_(lines.size()), data_{}, off_(off) {
    data_.reserve(w_ * h_);
    for (const auto& line: lines) {
      for (char c: line) {
        data_.push_back(tr(c));
      }
    }
  }

  Map(int w, int h, T init = {}, T off = {}) : w_(w), h_(h), data_(w * h, init), off_(off) {}

  value_type operator[](int x, int y) const {
    if (0 <= x && x < w_ && 0 <= y && y < h_) {
      return data_[y * w_ + x];
    }
    return 0;
  }
  value_type& operator[](int x, int y) {
    static value_type off;
    if (0 <= x && x < w_ && 0 <= y && y < h_) {
      return data_[y * w_ + x];
    }
    return off = off_;
  }
  int w() const { return w_; }
  int h() const { return h_; };

  auto iter() { return iter_(*this); }
  auto iter() const { return iter_(*this); }
};

struct Dir {
  int dx, dy;
};

struct Pos {
  int x, y;
  inline auto friend operator<=>(const Pos& a, const Pos& b) = default;
};

static const auto DIRS = std::vector<Dir> {
  { 0, -1},
  { 1, 0},
  { 0, 1},
  { -1, 0},
};

struct Place {
  int h;
  std::set<Pos> origins;
  int ways;
};

using PlaceMap = Map<Place>;
using Num = int;

// ------------------------------------------------------------------------ //

static std::pair<Num, Num>
solve(PlaceMap& map) {
  for (auto [x, y, p] : map.iter()) {
    if (p.h == 0) {
      p.origins.emplace(x, y);
      p.ways = 1;
    }
  }
  for (int h = 0; h < 9; ++h) {
    for (auto [x, y, p] : map.iter()) {
      if (p.h == h) {
        for (auto d: DIRS) {
          auto xx = x + d.dx;
          auto yy = y + d.dy;
          auto& pp = map[xx, yy];
          if (pp.h == h + 1) {
            pp.origins.insert(p.origins.begin(), p.origins.end());
            pp.ways += p.ways;
          }
        }
      }
    }
  }
  Num res1 = 0, res2 = 0;
  for (const auto [x, y, p] : map.iter()) {
    if (p.h == 9) {
      res1 += p.origins.size();
      res2 += p.ways;
    }
  }
  return {res1, res2};
}

// ------------------------------------------------------------------------ //

int
main() {
  std::vector<std::string> lines;
  std::string line;
  while (std::cin >> line) {
    lines.emplace_back(line);
  }
  auto map = PlaceMap(lines, [](auto c) { return Place{c - '0', {}, 0}; });

  auto [res1, res2] = solve(map);
  std::cout << "1: " << res1 << "\n";
  std::cout << "2: " << res2 << "\n";

  return 0;
}
