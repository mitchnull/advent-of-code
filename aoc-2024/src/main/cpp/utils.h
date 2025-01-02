#ifndef AOC_UTILS_H
#define AOC_UTILS_H

#include <string>
#include <functional>
#include <vector>
#include <ranges>
#include <iostream>

namespace views = std::views;

/* ------------------------------------------------------------------------ */

template<typename T>
std::size_t
hashCombine(std::size_t seed, const T& v) {
  return seed ^ (std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2));
}

/* ------------------------------------------------------------------------ */

struct Dir {
  int dx, dy;

  Dir& operator*=(auto n) {
    dx *= n;
    dy *= n;
    return *this;
  }
  friend Dir operator*(Dir d, auto n) {
    return d *= n;
  }
  friend Dir operator*(auto n, Dir d) {
    return d *= n;
  }
  Dir& operator/=(auto n) {
    dx /= n;
    dy /= n;
    return *this;
  }
  friend Dir operator/(Dir d, auto n) {
    return d /= n;
  }
  Dir operator-() {
    return Dir{-dx, -dy};
  }
  auto friend operator<=>(const Dir& a, const Dir& b) = default;

  friend std::ostream& operator<<(std::ostream& os, const Dir& d) {
    return os << "{" << d.dx << ", " << d.dy << "}";
  }
};

static const auto DIRS = std::vector<Dir> {
  {0, -1},
  {1, 0},
  {0, 1},
  {-1, 0},
};

template <>
struct std::hash<Dir> {
  std::size_t operator()(const Dir& d) const {
    return hashCombine(d.dx, d.dy);
  }
};

/* ------------------------------------------------------------------------ */

struct Pos {
  int x, y;

  Pos& operator+=(Dir d) {
    x += d.dx;
    y += d.dy;
    return *this;
  }
  friend Pos operator+(Pos p, Dir d) {
    return p += d;
  }
  Pos& operator-=(Dir d) {
    return (*this) += -d;
  }
  friend Pos operator-(Pos p, Dir d) {
    return p -= d;
  }
  friend Dir operator-(Pos a, Pos b) {
    return {a.x - b.x, a.y - b.y};
  }
  auto friend operator<=>(const Pos& a, const Pos& b) = default;

  friend std::ostream& operator<<(std::ostream& os, const Pos& p) {
    return os << "{" << p.x << ", " << p.y << "}";
  }
};

template <>
struct std::hash<Pos> {
  std::size_t operator()(const Pos& p) const {
    return hashCombine(p.x, p.y);
  }
};

/* ------------------------------------------------------------------------ */

template<typename T = char>
struct Grid {
  using value_type = std::conditional<std::is_same<T, bool>::value, char, T>::type;

  template<typename Tr = std::identity>
  Grid(std::vector<std::string> lines, value_type off = {}, Tr tr = {}) : w_(lines[0].size()), h_(lines.size()), data_{}, off_(off) {
    data_.reserve(w_ * h_);
    for (const auto& line: lines) {
      for (char c: line) {
        data_.push_back(tr(c));
      }
    }
  }

  Grid(int w, int h, T init = {}, T off = {}) : w_(w), h_(h), data_(w * h, init), off_(off) {}

  const value_type& operator[](int x, int y) const {
    if (0 <= x && x < w_ && 0 <= y && y < h_) {
      return data_[y * w_ + x];
    }
    return off_;
  }
  value_type& operator[](int x, int y) {
    static value_type off;
    if (0 <= x && x < w_ && 0 <= y && y < h_) {
      return data_[y * w_ + x];
    }
    return off = off_;
  }
  const value_type& operator[](Pos pos) const { return (*this)[pos.x, pos.y]; }
  value_type& operator[](Pos pos) { return (*this)[pos.x, pos.y]; }

  int w() const { return w_; }
  int h() const { return h_; };

  auto iter() { return iter_(*this); }
  auto iter() const { return iter_(*this); }
  auto begin() { return data_.begin(); }
  auto begin() const { return data_.begin(); }
  auto end() { return data_.end(); }
  auto enn() const { return data_.end(); }

  friend std::ostream& operator<<(std::ostream& os, const Grid& m) {
    for (int y = 0; y < m.h(); ++y) {
      for (int x = 0; x < m.w(); ++x) {
        os << m[x, y];
      }
      os << "\n";
    }
    return os;
  }
private:
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
};

/* ------------------------------------------------------------------------ */

#endif /* AOC_UTILS_H */
