#include <iostream>
#include <string>
#include <ranges>
#include <map>

namespace views = std::views;

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
  auto friend operator<=>(const Dir& a, const Dir& b) = default;

  friend std::ostream& operator<<(std::ostream& os, const Dir& d) {
    return os << "{" << d.dx << ", " << d.dy << "}";
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

  auto friend operator<=>(const Pos& a, const Pos& b) = default;

  friend std::ostream& operator<<(std::ostream& os, const Pos& p) {
    return os << "{" << p.x << ", " << p.y << "}";
  }
};

/* ------------------------------------------------------------------------ */

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

  friend std::ostream& operator<<(std::ostream& os, const Map& m) {
    for (int y = 0; y < m.h(); ++y) {
      for (int x = 0; x < m.w(); ++x) {
        os << m[x, y];
      }
      os << "\n";
    }
    return os;
  }
};

using Board = Map<char>;
using Num = int;

struct PosDir {
  Pos pos;
  Dir dir;

  auto friend operator<=>(const PosDir& a, const PosDir& b) = default;
};

struct Node {
  PosDir v;
  Num cost;

  friend bool operator<(const Node&a, const Node&b) { return a.cost > b.cost; };
};

using Queue = std::priority_queue<Node>;

static auto
pop(Queue& queue) {
  auto res = queue.top();
  queue.pop();
  return res;
}

/* ------------------------------------------------------------------------ */

static Num
solve1(const Board& board, PosDir start) {
  auto costs = std::map<PosDir, Num>();
  auto queue = Queue();
  queue.push(Node{{start}, 0});
  while (!queue.empty()) {
    Node n = pop(queue);
    auto cp = costs.find(n.v);
    if (cp != costs.end() && n.cost >= cp->second) {
      continue;
    }
    costs[n.v] = n.cost;
    if (board[n.v.pos] == 'E') {
      return n.cost;
    }
    auto np = n.v.pos + n.v.dir;
    if (board[np] != '#') {
      queue.emplace(PosDir{np, n.v.dir}, n.cost + 1);
    }
    Dir rot = Dir{n.v.dir.dy, n.v.dir.dx};
    queue.emplace(PosDir{n.v.pos, rot}, n.cost + 1000);
    queue.emplace(PosDir{n.v.pos, -rot}, n.cost + 1000);
  }
  return 0;
}

/* ------------------------------------------------------------------------ */

int
main() {
  std::vector<std::string> lines;
  std::string line;
  while (std::getline(std::cin, line)) {
    lines.push_back(line);
  }
  Board board = Board(lines);
  Pos start = (board.iter() | views::filter([](auto i) { return i.v == 'S'; }) | views::transform([](auto i) { return Pos{i.x, i.y}; })).front();

  auto res1 = solve1(board, {start, {1, 0}});
  // auto res2 = solve2(lines, moves);

  std::cout << "1: " << res1 << "\n";
  // std::cout << "2: " << res2 << "\n";

  return 0;
}
