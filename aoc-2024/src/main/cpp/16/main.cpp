#include <iostream>
#include <string>
#include <ranges>
#include <unordered_map>
#include <unordered_set>

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

struct PosDir {
  Pos pos;
  Dir dir;

  auto friend operator<=>(const PosDir& a, const PosDir& b) = default;
};

template <>
struct std::hash<PosDir> {
  std::size_t operator()(const PosDir& pd) const {
    return std::hash<Pos>()(pd.pos) * 11 + std::hash<Dir>()(pd.dir);
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


struct Node {
  PosDir v;
  Num cost;

  friend bool operator<(const Node&a, const Node&b) { return a.cost > b.cost; };
};

using Queue = std::priority_queue<Node>;

/* ------------------------------------------------------------------------ */

static void
findBestPlaces(const std::unordered_map<PosDir, std::unordered_set<PosDir>>& prevs, PosDir pd, std::unordered_set<PosDir>& bestPlaces) {
  if (bestPlaces.contains(pd)) {
    return;
  }
  bestPlaces.insert(pd);

  auto pp = prevs.find(pd);
  if (pp == prevs.end()) {
    return;
  }
  for (auto pd : pp->second) {
    findBestPlaces(prevs, pd, bestPlaces);
  }
}

static Num
findCost(const auto& costs, const PosDir& v) {
  auto cp = costs.find(v);
  return cp != costs.end() ? cp->second : std::numeric_limits<Num>::max();
}

static std::pair<Num, Num>
solve(const Board& board, PosDir start, Pos end) {
  auto costs = std::unordered_map<PosDir, Num>();
  auto prevs = std::unordered_map<PosDir, std::unordered_set<PosDir>>();
  auto queue = Queue();
  queue.emplace(start, 0);
  while (!queue.empty()) {
    Node n = queue.top();
    queue.pop();
    if (n.cost != costs[n.v]) {
      continue;
    }
    for (Node nn : {
        Node{PosDir{n.v.pos + n.v.dir, n.v.dir}, n.cost + 1},
        Node{PosDir{n.v.pos, Dir{n.v.dir.dy, n.v.dir.dx}}, n.cost + 1000},
        Node{PosDir{n.v.pos, Dir{-n.v.dir.dy, -n.v.dir.dx}}, n.cost + 1000}}) {
      auto cc = findCost(costs, nn.v);
      if (board[nn.v.pos] == '#' || nn.cost > cc) {
        continue;
      }
      if (nn.cost < cc) {
        costs[nn.v] = nn.cost;
        prevs[nn.v] = {n.v};
      } else {
        prevs[nn.v].insert(n.v);
      }
      queue.push(nn);
    }
  }

  Num minCost = std::numeric_limits<Num>::max();
  auto bestPlaces = std::unordered_set<PosDir>{};
  for (auto dir : DIRS) {
    auto endDir = PosDir{end, dir};
    auto cp = costs.find(endDir);
    if (cp != costs.end() && cp->second < minCost) {
      minCost = cp->second;
      bestPlaces.clear();
    }
    if (cp->second == minCost) {
      findBestPlaces(prevs, endDir, bestPlaces);
    }
  }
  auto numBestPlaces = (bestPlaces | views::transform([](auto pd) { return pd.pos; }) | std::ranges::to<std::unordered_set<Pos>>()).size();
  return {minCost, numBestPlaces};
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
  Pos end = (board.iter() | views::filter([](auto i) { return i.v == 'E'; }) | views::transform([](auto i) { return Pos{i.x, i.y}; })).front();

  auto [res1, res2] = solve(board, {start, {1, 0}}, end);

  std::cout << "1: " << res1 << "\n";
  std::cout << "2: " << res2 << "\n";

  return 0;
}
