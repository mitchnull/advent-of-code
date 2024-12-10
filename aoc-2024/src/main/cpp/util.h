#include <string>
#include <functional>

template<typename T>
class Map {
  using value_type = std::conditional<std::is_same<T, bool>::value, char, T>::type;
  int w_, h_;
  std::vector<value_type> data_;
  value_type off_;
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
};

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

static const auto DIRS = std::vector<Dir> {
  { 0, -1},
  { 1, 0},
  { 0, 1},
  { -1, 0},
};

