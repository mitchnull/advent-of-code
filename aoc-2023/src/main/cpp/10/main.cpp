#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

using Num = std::int64_t;

enum Dir {
  Z = 0,
  U = 1,
  D = 2,
  L = 4,
  R = 8,
  UD = U | D,
  UL = U | L,
  UR = U | R,
  DL = D | L,
  DR = D | R,
  LR = L | R,
  S = 127,
};

static Dir
toDir(char c) {
  switch (c) {
    case '.': return Z;
    case '|': return UD;
    case '-': return LR;
    case '7': return DL;
    case 'F': return DR;
    case 'J': return UL;
    case 'L': return UR;
    case 'S': return S;
  }
  return Z;
}

static Dir rev(Dir dir) {
  switch (dir) {
    case U: return D;
    case D: return U;
    case L: return R;
    case R: return L;
    default: return dir;
  }
}

struct Pos {
  int i, j;
};

static Pos
step(Pos p, Dir dir) {
  switch (dir) {
    case U: --p.i; return p;
    case D: ++p.i; return p;
    case L: --p.j; return p;
    case R: ++p.j; return p;
    default: return p;
  }
}

template<typename T>
struct Map {
  std::vector<std::vector<T>> lines;

  T operator()(int i, int j) const {
    if (i < 0 || i >= (int)lines.size()) {
      return {};
    }
    auto& line = lines[i];
    if (j < 0 || j >= (int)line.size()) {
      return {};
    }
    return line[j];
  }

  T& operator()(int i, int j) {
    static T def{};
    if (i < 0 || i >= (int)lines.size()) {
      return def;
    }
    auto& line = lines[i];
    if (j < 0 || j >= (int)line.size()) {
      return def;
    }
    return line[j];
  }
  
  template <typename U>
  Map& resize(const Map<U>& m) {
    lines.resize(m.lines.size());
    for (int i = 0, e = lines.size(); i != e; ++i) {
      lines[i].resize(m.lines[i].size());
    }
    return *this;
  }

  Map& resize(int w, int h) {
    lines.resize(h);
    for (int i = 0, e = lines.size(); i != e; ++i) {
      lines[i].resize(w);
    }
    return *this;
  }
};

using ScaledMap = Map<std::uint8_t>;

static const std::vector<Dir> Dirs{U, D, L, R};

static const auto Figs = std::vector<ScaledMap>{
    {{{0,1,0},
      {0,1,0},
      {0,0,0}}},

    {{{0,0,0},
      {0,1,0},
      {0,1,0}}},

    {{{0,0,0},
      {1,1,0},
      {0,0,0}}},

    {{{0,0,0},
      {0,1,1},
      {0,0,0}}},
  };

static Pos
findStart(const Map<Dir>& map) {
  for (int i = 0, e = map.lines.size(); i < e; ++i) {
    const auto& line = map.lines[i];
    for (int j = 0, e = line.size(); j < e; ++j) {
      if (line[j] == S) {
        return {i, j};
      }
    }
  }
  return {-1, -1};
}

static void
scaleUp(const Map<Dir>& map, Pos p, Dir curr, ScaledMap& scaledMap) {
  for (int di = 0; di < Dirs.size(); ++di) {
    Dir dir = Dirs[di];
    if (curr & dir) {
      auto& fig = Figs[di];
      for (int i = 0 ; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
          scaledMap(p.i * 3 + i, p.j * 3 + j) |= fig(i, j);
        }
      }
    }
  }
}

static int
findDist(const Map<Dir>& map, Map<int>& dists, ScaledMap& scaledMap) {
  Pos p = findStart(map);
  int count = 0;
  while (dists(p.i, p.j) == 0) {
    dists(p.i, p.j) = ++count;
    Dir curr = map(p.i, p.j);
    scaleUp(map, p, curr, scaledMap);

    for (Dir dir : Dirs) {
      if (!(curr & dir)) {
        continue;
      }
      Pos pp = step(p, dir);
      if (dists(pp.i, pp.j) == 0 && (map(pp.i, pp.j) & rev(dir))) {
        p = pp;
        break;
      }
    }
  }
  return count / 2;
}

static void
fill(ScaledMap& scaledMap, Pos p) {
  if (scaledMap(p.i, p.j)) {
    return;
  }
  scaledMap(p.i, p.j) = true;
  for (Dir dir : Dirs) {
    fill(scaledMap, step(p, dir));
  }
}

int
main() {
  Map<Dir> map;
  Map<int> dists;
  std::string line;
  while (std::getline(std::cin, line)) {
    std::vector<Dir> dirs;
    std::transform(line.begin(), line.end(), std::back_inserter(dirs), toDir);
    map.lines.push_back(std::move(dirs));
  }
  dists.resize(map);
  int w = map.lines.front().size();
  int h = map.lines.size();
  ScaledMap scaledMap;
  scaledMap.resize(w * 3, h * 3);
  int dist = findDist(map, dists, scaledMap);
  fill(scaledMap, {0, 0});

  int inner = 0;
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      inner += !scaledMap(i * 3 + 1, j * 3 + 1);
    }
  }
  std::cout << dist << "\n";
  std::cout << inner << "\n";
}
