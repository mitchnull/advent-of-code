#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

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
  return 5;
}

static std::pair<Pos, Dir>
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

int
main() {
  // const int w = 7, h = 11;
  const int w = 101, h = 103;
  auto quadrants = std::vector<int>(5);
  std::string line;
  while (std::getline(std::cin, line)) {
    auto [p, d] = parse(line);
    for (int i = 0; i < 100; ++i) {
      p = {(p.x + d.dx + w) % w, (p.y + d.dy + h) % h};
    }
    ++quadrants[quad(p, w, h)];
  }
  int res1 = std::reduce(quadrants.begin(), quadrants.begin() + 4, 1, std::multiplies<>());

  std::cout << "1: " << res1 << "\n";
  // std::cout << "2: " << res2 << "\n";
  return 0;
}
