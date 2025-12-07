#ifndef AOC_UTILS_H
#define AOC_UTILS_H

#include <string>
#include <functional>
#include <vector>
#include <ranges>
#include <iostream>
#include <print>

namespace views = std::views;
namespace ranges = std::ranges;
using std::print, std::println;

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

template <>
struct std::formatter<Dir> {
  constexpr auto parse(std::format_parse_context& ctx) {
      return ctx.begin();
  }

  auto format(const Dir& d, std::format_context& ctx) const {
      return std::format_to(ctx.out(), "{{{}, {}}}", d.dx, d.dy);
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

template <>
struct std::formatter<Pos> {
  constexpr auto parse(std::format_parse_context& ctx) {
      return ctx.begin();
  }

  auto format(const Pos& pos, std::format_context& ctx) const {
      return std::format_to(ctx.out(), "{{{}, {}}}", pos.x, pos.y);
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
  auto end() const { return data_.end(); }

  friend std::ostream& operator<<(std::ostream& os, const Grid& m) {
    for (int y = 0; y < m.h(); ++y) {
      for (int x = 0; x < m.w(); ++x) {
        os << m[x, y];
      }
      os << "\n";
    }
    return os;
  }

  template<typename Tr = std::identity>
  static Grid read(std::istream& in = std::cin, value_type off = {}, Tr tr = {}) {
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(in, line)) {
      lines.push_back(line);
    }
    return Grid(lines, off, tr);
  }

  auto friend operator<=>(const Grid& a, const Grid& b) = default;
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

template <typename T>
struct std::hash<Grid<T>> {
  std::size_t operator()(const Grid<T>& grid) const {
    std::size_t h = hashCombine(grid.w(), grid.h());
    for (const auto& v: grid) {
      h = hashCombine(h, v);
    }
    return h;
  }
};

template <typename T>
struct std::formatter<Grid<T>> {
  constexpr auto parse(std::format_parse_context& ctx) {
      return ctx.begin();
  }

  auto format(const Grid<T>& m, std::format_context& ctx) const {
    for (int y = 0; y < m.h(); ++y) {
      for (int x = 0; x < m.w(); ++x) {
        std::format_to(ctx.out(), "{}", m[x, y]);
      }
      std::format_to(ctx.out(), "\n");
    }
    return ctx.out();
  }
};

/* ------------------------------------------------------------------------ */

static auto
piecewise_apply(auto a, auto b, auto op) {
  return [&]<size_t... Is>(std::index_sequence<Is...>) {
    return std::make_tuple(op(std::get<Is>(a), std::get<Is>(b))...);
  }(std::make_index_sequence<std::tuple_size_v<decltype(a)>>());
}

template <typename... As, typename... Bs>
static auto
operator+(const std::tuple<As...>& a, const std::tuple<Bs...>& b) {
  return piecewise_apply(a, b, std::plus<>());
}

template <typename... As, typename... Bs>
static auto
operator+=(std::tuple<As...>& a, const std::tuple<Bs...>& b) {
  return a = piecewise_apply(a, b, std::plus<>());
}

template <typename A, typename B>
static auto
operator+(const std::pair<A, B>& a, const std::pair<A, B>& b) {
  return piecewise_apply(a, b, std::plus<>());
}

template <typename A, typename B>
static auto
operator+=(std::pair<A, B>& a, const std::pair<A, B>& b) {
  return a = piecewise_apply(a, b, std::plus<>());
}

/* ------------------------------------------------------------------------ */

#endif /* AOC_UTILS_H */
