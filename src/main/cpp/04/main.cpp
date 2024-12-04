#include <iostream>
#include <cstdint>
#include <string>

using uint = std::uint32_t;

class Map {
  std::vector<std::string> lines_;
  int w_, h_;
public:
  Map(std::vector<std::string> lines) : lines_(lines), w_(lines[0].size()), h_(lines.size()) {}
  
  char operator()(int x, int y) const {
    if (0 <= x && x < w_ && 0 <= y && y < h_) {
      return lines_[y][x];
    }
    return 0;
  }
  int w() const { return w_; }
  int h() const { return h_; };
};

struct Dir {
  int dx, dy;
};

const auto DIRS = std::vector<Dir> {
  { 0, 1},
  { 1, 1},
  { 1, 0},
  { 1, -1},
  { 0, -1},
  { -1, -1},
  { -1, 0},
  { -1, 1},
};

const auto DIRSX = std::vector<Dir> {
  { 1, 1},
  { 1, -1},
  { -1, -1},
  { -1, 1},
};

// ------------------------------------------------------------------------ //

static bool
matches1(const Map& map, int x, int y, Dir d, std::string_view word) {
  for (int i = 0; i < word.size(); ++i, x += d.dx, y += d.dy) {
    if (map(x, y) != word[i]) {
      return false;
    }
  }
  return true;
}

static int
count1(const Map& map, std::string_view word) {
  int res = 0;
  for (int x = 0; x < map.w(); ++x) {
    for (int y = 0; y < map.h(); ++y) {
      for (const auto& d : DIRS) {
        res += matches1(map, x, y, d, word);
      }
    }
  }
  return res;
}

// ------------------------------------------------------------------------ //

static bool
matches2(const Map& map, int x, int y, std::string_view word) {
  int m = word.size() / 2;
  for (const auto& d : DIRSX) {
    auto d1 = Dir{d.dy, -d.dx};
    auto d2 = Dir{-d.dy, d.dx};
    if (matches1(map, x - m * d.dx, y - m * d.dy, d, word) &&
        (matches1(map, x - m * d1.dx, y - m * d1.dy, d1, word) ||
         matches1(map, x - m * d2.dx, y - m * d2.dy, d2, word))) {
      return true;
    }
  }
  return false;
}


static int
count2(const Map& map, std::string_view word) {
  int hwl = word.size() / 2;
  int res = 0;
  for (int x = hwl; x < map.w() - hwl; ++x) {
    for (int y = hwl; y < map.h() - hwl; ++y) {
      res += matches2(map, x, y, word);
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
  auto map = Map(lines);

  int res1 = count1(map, "XMAS");
  std::cout << "1: " << res1 << "\n";

  // ------------------------------------------------------------------------ //

  int res2 = count2(map, "MAS");
  std::cout << "2: " << res2 << "\n";

  return 0;
}
