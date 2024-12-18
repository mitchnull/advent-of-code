#include <iostream>
#include <string>
#include <ranges>

namespace views = std::ranges::views;

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

template <>
struct std::hash<Dir> {
  std::size_t operator()(const Dir& d) const {
    return d.dx * 11 + d.dy;
  }
};

static const auto DIRS = std::vector<Dir> {
  {0, -1},
  {1, 0},
  {0, 1},
  {-1, 0},
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

template <>
struct std::hash<Pos> {
  std::size_t operator()(const Pos& p) const {
    return p.x * 11 + p.y;
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

/* ------------------------------------------------------------------------ */

using Board = Map<char>;
using Num = int;

struct Node {
  Pos pos;
  Num cost;

  friend bool operator<(const Node&a, const Node&b) { return a.cost > b.cost; };
};

using Queue = std::priority_queue<Node>;

/* ------------------------------------------------------------------------ */

static std::pair<Num, Board>
solve(const Board& board, Pos start, Pos end) {
  auto path = Board(board);
  auto costs = Map<Num>(board.w(), board.h(), std::numeric_limits<Num>::max(), std::numeric_limits<Num>::max());
  auto prevs = Map<Pos>(board.w(), board.h(), {-1, -1});
  auto queue = Queue();
  queue.emplace(start, 0);
  costs[start] = 0;
  while (!queue.empty()) {
    Node n = queue.top();
    queue.pop();
    if (n.pos == end) {
      for (Pos p = n.pos; p != Pos{-1, -1}; p = prevs[p]) {
        path[p] = 'O';
      }
      return {n.cost, path};
    }
    if (n.cost != costs[n.pos]) {
      continue;
    }
    for (Dir d : DIRS) {
      auto nn = Node{n.pos + d, n.cost + 1};
      auto cc = costs[nn.pos];
      if (board[nn.pos] == '#' || nn.cost >= cc) {
        continue;
      }
      costs[nn.pos] = nn.cost;
      prevs[nn.pos] = n.pos;
      queue.push(nn);
    }
  }
  return {-1, path};
}

/* ------------------------------------------------------------------------ */

int
main() {
  const Pos startPos = {0, 0};
  const Pos endPos = {70, 70};
  const int count = 1024;

  std::vector<Pos> blocks;
  Pos p;
  char c;
  while (std::cin >> p.x >> c >>  p.y) {
    blocks.push_back(p);
  }
  Board board = Board(endPos.x + 1, endPos.y + 1, '.', '#');
  for (auto it = blocks.begin(), end = blocks.begin() + count; it != end; ++it) {
    board[*it] = '#';
  }

  auto [res1, path] = solve(board, startPos, endPos);
  std::cout << "1: " << res1 << "\n";

  for (auto it = blocks.begin() + count, end = blocks.end(); it != end; ++it) {
    board[*it] = '#';
    if (path[*it] != 'O') {
      continue;
    }
    auto [cost, pp] = solve(board, startPos, endPos);
    if (cost < 0) {
      std::cout << it->x << "," << it->y << "\n";
      return 0;
    }
    std::swap(path, pp);
  }

  return 0;
}
