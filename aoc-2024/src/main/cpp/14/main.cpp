#include <iostream>
#include <string>
#include <sstream>
#include <numeric>
#include <bitset>

using Row = std::bitset<128>;
using Map = std::vector<Row>;

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
  inline auto friend operator<=>(const Dir& a, const Dir& b) = default;
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

struct Robot {
  Pos p;
  Dir v;

  inline auto friend operator<=>(const Robot& a, const Robot& b) = default;
};

using Robots = std::vector<Robot>;

static Robot
parse(std::string line) {
  // p=0,4 v=3,-3
  char c;
  Pos p;
  Dir d;
  auto ss = std::stringstream{line};
  ss >> c >> c >> p.x >> c >> p.y >> c >> c >> d.dx >> c >> d.dy;
  return {p, d};
}

/* ------------------------------------------------------------------------ */

static int
quad(const Pos& p, int w, int h) {
  if (p.x < w / 2) {
    if (p.y < h / 2) {
      return 0;
    } else if (p.y > h / 2) {
      return 1;
    }
  } else if (p.x > w / 2) {
    if (p.y < h / 2) {
      return 2;
    } else if (p.y > h / 2) {
      return 3;
    }
  }
  return 4;
}

/* ------------------------------------------------------------------------ */

static void
render(const Map& map, int w, int h, int i) {
  std::cout << i << ":\n";
  for (int y = 0; y < h; y += 2) {
    for (int x = 0; x < w; ++x) {
      if (map[y][x]) {
        if ((y + 1) < h && map[y + 1][x]) {
          std::cout << "█";
        } else {
          std::cout << "▀";
        }
      } else if ((y + 1) < h && map[y + 1][x]) {
        std::cout << "▄";
      } else {
        std::cout << " ";
      }
    }
    std::cout << "\n";
  }
}

static Map
render(const Robots& robots, int w, int h) {
  auto map = Map(h);
  for (const auto& r : robots) {
    map[r.p.y][r.p.x] = true;
  }
  return map;
}

static bool
isTree(const Map& map, int w, int h) {
  constexpr const int len = 22;
  for (auto row: map) {
    Row mask((1UL << (len + 1)) - 1);
    for (int i = 0; i < w - len; ++i, mask <<= 1) {
      if ((row & mask) == mask) {
        return true;
      }
    }
  }
  return false;
}

static int
findTree(Robots& robots, int w, int h) {
  Map map, startMap = render(robots, w, h);
  for (int res = 1; map != startMap; ++res) {
    for (auto& r : robots) {
      r.p = {(r.p.x + r.v.dx + w) % w, (r.p.y + r.v.dy + h) % h};
    }
    map = render(robots, w, h);
    if (isTree(map, w, h)) {
      render(map, w, h, res);
      return res;
    }
  }
  return 0;
}

/* ------------------------------------------------------------------------ */

int
main() {
  // const int w = 7, h = 11;
  const int w = 101, h = 103;
  auto quadrants = std::vector<int>(5);
  auto robots = Robots{};

  std::string line;
  while (std::getline(std::cin, line)) {
    auto r = parse(line);
    robots.push_back(r);
    for (int i = 0; i < 100; ++i) {
      r.p = {(r.p.x + r.v.dx + w) % w, (r.p.y + r.v.dy + h) % h};
    }
    ++quadrants[quad(r.p, w, h)];
  }

  int res1 = std::reduce(quadrants.begin(), quadrants.begin() + 4, 1, std::multiplies<>());
  int res2 = findTree(robots, w, h);

  std::cout << "1: " << res1 << "\n";
  std::cout << "2: " << res2 << "\n";
  return 0;
}
