#include <iostream>
#include <cstdint>
#include <string>

using uint = std::uint32_t;

class Map {
  std::vector<std::string> lines_;
  int w_, h_;
public:
  Map(std::vector<std::string> lines) : lines_(lines), w_(lines[0].size()), h_(lines.size()) {}

  Map(int w, int h, char c = '\0') : lines_(h, std::string(w, c)), w_(w), h_(h) {}

  char operator()(int x, int y) const {
    if (0 <= x && x < w_ && 0 <= y && y < h_) {
      return lines_[y][x];
    }
    return 0;
  }
  char& operator()(int x, int y) {
    static char off;
    if (0 <= x && x < w_ && 0 <= y && y < h_) {
      return lines_[y][x];
    }
    return off = 0;
  }
  int w() const { return w_; }
  int h() const { return h_; };
};

struct Dir {
  int dx, dy;
};

struct Pos {
  int x, y, d;
};

const auto DIRS = std::vector<Dir> {
  { 0, -1},
  { 1, 0},
  { 0, 1},
  { -1, 0},
};

// ------------------------------------------------------------------------ //

static Pos
findStart(const Map& map) {
  for (int x = 0; x < map.w(); ++x) {
    for (int y = 0; y < map.h(); ++y) {
      if (map(x, y) == '^') {
        return {x, y, 0};
      }
    }
  }
  return {};
}

static Pos
move(const Map& map, const Pos& pos, Map& visited) {
  visited(pos.x, pos.y) |= 1 << pos.d;
  Dir d = DIRS[pos.d];
  Pos np = {pos.x + d.dx, pos.y + d.dy, pos.d};
  if (map(np.x, np.y) == '#') {
    return move(map, {pos.x, pos.y, static_cast<int>((pos.d + 1) % DIRS.size())}, visited);
  }
  return np;
}

static int
countVisited(const Map& map) {
  int res = 0;
  for (int x = 0; x < map.w(); ++x) {
    for (int y = 0; y < map.h(); ++y) {
      if (map(x, y)) {
        ++res;
      }
    }
  }
  return res;
}

static int
countSteps(const Map& map, Pos pos, Map& visited) {
  while (map(pos.x, pos.y)) {
    pos = move(map, pos, visited);
  }
  return countVisited(visited);
}

// ------------------------------------------------------------------------ //

static bool
isLoop(const Map& map, Pos pos) {
  auto visited = Map(map.w(), map.h());
  while (map(pos.x, pos.y)) {
    if (visited(pos.x, pos.y) & (1 << pos.d)) {
      return true;
    }
    pos = move(map, pos, visited);
  }
  return false;
}

static int
countLoops(Map map, const Pos& start, const Map& visited) {
  int res = 0;
  for (int x = 0; x < map.w(); ++x) {
    for (int y = 0; y < map.h(); ++y) {
      if (visited(x, y) && map(x, y) == '.') {
        map(x, y) = '#';
        if (isLoop(map, start)) {
          ++res;
        }
        map(x, y) = '.';
      }
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
  const auto map = Map(lines);
  const auto start = findStart(map);

  auto visited = Map(map.w(), map.h());
  int res1 = countSteps(map, start, visited);
  std::cout << "1: " << res1 << "\n";

  int res2 = countLoops(map, start, visited);
  std::cout << "2: " << res2 << "\n";

  return 0;
}
