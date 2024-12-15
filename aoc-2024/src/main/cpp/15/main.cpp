#include <iostream>
#include <string>
#include <numeric>
#include <ranges>

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


/* ------------------------------------------------------------------------ */

static Dir
dir(char c) {
  switch (c) {
    case '^': return {0, -1};
    case '>': return {1, 0};
    case 'v': return {0, 1};
    case '<': return {-1, 0};
  }
  return {};
}

static Pos
move(Board& board, Pos p, Dir d) {
  auto pp = p + d;
  switch (board[pp]) {
    case '#':
      return p;
    case 'O':
      if (move(board, pp, d) == pp) {
        return p;
      }
  }
  std::swap(board[pp], board[p]);
  return pp;
}

static int
solve1(const std::vector<std::string>& lines, std::vector<char>& moves) {
  Board board = Board(lines);
  Pos p = (board.iter() | views::filter([](auto i) { return i.v == '@'; }) | views::transform([](auto i) { return Pos{i.x, i.y}; })).front();
  for (char c : moves) {
    p = move(board, p, dir(c));
  }
  auto gpsScores = board.iter() | views::filter([](auto i) { return i.v == 'O'; }) | views::transform([](auto i) { return i.y * 100 + i.x; });
  return std::reduce(gpsScores.begin(), gpsScores.end(), 0);
}

/* ------------------------------------------------------------------------ */

static char
left(char c) {
  switch (c) {
    case 'O': return '[';
    case '@': return '@';
  }
  return c;
}

static char
right(char c) {
  switch (c) {
    case 'O': return ']';
    case '#': return '#';
    case '@': return '.';
  }
  return c;
}

static Pos
otherSide(char c, Pos p) {
  switch (c) {
    case '[': return {p.x + 1, p.y};
    case ']': return {p.x - 1, p.y};
  }
  return p;
}

static Pos
move2(Board& board, Pos p, Dir d) {
  auto po = d.dy == 0 ? p : otherSide(board[p], p);
  auto pn = p + d;
  auto pon = po + d;

  if (board[pn] == '#' || board[pon] == '#') {
    return p;
  }
  if (board[pn] == '.' && board[pon] == '.') {
    if (po != p) {
      std::swap(board[po], board[pon]);
    }
    std::swap(board[p], board[pn]);
    return pn;
  }
  Board save = board;
  if ((board[pn] == '.' || move2(board, pn, d) != pn) && (board[pon] == '.' || move2(board, pon, d) != pon)) {
    return move2(board, p, d);
  }
  board = save;
  return p;
}

static int
solve2(const std::vector<std::string>& lines, std::vector<char>& moves) {
  Board board = Board(lines.front().size() * 2, lines.size());
  for (int y = 0; y < lines.size(); ++y) {
    const auto& line = lines[y];
    for (int x = 0; x < line.size(); ++x) {
      board[2 * x, y] = left(line[x]);
      board[2 * x + 1, y] = right(line[x]);
    }
  }

  Pos p = (board.iter() | views::filter([](auto i) { return i.v == '@'; }) | views::transform([](auto i) { return Pos{i.x, i.y}; })).front();
  for (char c : moves) {
    p = move2(board, p, dir(c));
  }
  auto gpsScores = board.iter() | views::filter([](auto i) { return i.v == '['; }) | views::transform([](auto i) { return i.y * 100 + i.x; });
  return std::reduce(gpsScores.begin(), gpsScores.end(), 0);
}

/* ------------------------------------------------------------------------ */

int
main() {
  std::vector<std::string> lines;
  std::string line;
  while (std::getline(std::cin, line) && !line.empty()) {
    lines.push_back(line);
  }
  auto moves = std::vector<char>(std::istream_iterator<char>(std::cin), std::istream_iterator<char>());

  int res1 = solve1(lines, moves);
  int res2 = solve2(lines, moves);

  std::cout << "1: " << res1 << "\n";
  std::cout << "2: " << res2 << "\n";

  return 0;
}
