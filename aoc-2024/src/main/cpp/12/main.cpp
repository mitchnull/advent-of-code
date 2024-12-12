#include <iostream>
#include <cstdint>
#include <string>

using Num = uint64_t;

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
  Dir operator-() {
    return Dir{-dx, -dy};
  }
};

static const auto DIRS = std::vector<Dir> {
  { 0, -1},
  { 1, 0},
  { 0, 1},
  { -1, 0},
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

  inline auto friend operator<=>(const Pos& a, const Pos& b) = default;
};

/* ------------------------------------------------------------------------ */

template<typename T>
class Map {
  using value_type = std::conditional<std::is_same<T, bool>::value, char, T>::type;
  int w_, h_;
  std::vector<value_type> data_;
  const value_type off_;

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
};

/* ------------------------------------------------------------------------ */

using Plants = Map<char>;
using BoolMap = Map<bool>;
using IntMap = Map<int>;

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
      if (visited[x, y]) {
        continue;
      }
      char plant = plants[x, y];
      Num area = 0, perimiter = 0;
      std::deque<Pos> queue{ {x, y} };
      ++id;
      while (!queue.empty()) {
        auto p = pop_front(queue);
        if (visited[p.x, p.y]) {
          continue;
        }
        visited[p.x, p.y] = true;
        ids[p.x, p.y] = id;
        ++area;
        for (auto d: DIRS) {
          auto pp = p + d;
          if (plants[pp.x, pp.y] != plant) {
            ++perimiter;
          } else if (!visited[pp.x, pp.y]) {
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
