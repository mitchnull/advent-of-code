#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <iterator>
#include <string>
#include <numeric>

using Num = std::uint64_t;
using Button = char;
using Buttons = std::vector<Button>;
using Edge = std::pair<Button, Button>;
using Path = std::string;
using Paths = std::vector<Path>;
using SPaths = std::unordered_map<Edge, Paths>;
using ChunkedPath = std::unordered_map<Path, Num>;
using ChunkedPaths = std::vector<ChunkedPath>;

template <>
struct std::hash<Edge> {
  std::size_t operator()(const Edge& e) const {
    return e.first * 11 + e.second;
  }
};

static const auto ABUTTONS = std::vector {
  '7', '8', '9',
  '4', '5', '6',
  '1', '2', '3',
  'X', '0', 'A',
};

static const auto BBUTTONS = std::vector {
  'X', '^', 'A',
  '<', 'v', '>',
};

/* ------------------------------------------------------------------------ */

static std::ostream&
operator<<(auto& os, const ChunkedPath& chunks) {
  os << "{\n";
  for (auto [k, v] : chunks) {
    os << "  " << k << ": " << v << ",\n";
  }
  return os << "}\n";
}

static std::ostream&
operator<<(auto& os, const Paths& paths) {
  os << "{\n";
  for (auto p : paths) {
    os << "  " << p << ",\n";
  }
  return os << "}\n";
}

static Paths
findBasicPaths(const Buttons& buttons, Button f, Button t) {
  if (f == 'X' || t == 'X') {
    return {};
  }
  if (f == t) {
    return {Path{}};
  }
  auto fi = std::find(buttons.begin(), buttons.end(), f) - buttons.begin();
  auto fx = fi % 3;
  auto fy = fi / 3;
  auto ti = std::find(buttons.begin(), buttons.end(), t) - buttons.begin();
  auto tx = ti % 3;
  auto ty = ti / 3;
  auto dx = tx - fx;
  auto dy = ty - fy;
  Path xp;
  Path yp;
  while (dx > 0) {
    xp += '>';
    --dx;
  }
  while (dx < 0) {
    xp += '<';
    ++dx;
  }
  while (dy > 0) {
    yp += 'v';
    --dy;
  }
  while (dy < 0) {
    yp += '^';
    ++dy;
  }
  if (xp.empty()) {
    return {yp};
  }
  if (yp.empty()) {
    return {xp};
  }
  Paths paths;
  if (buttons[fy * 3 + tx] != 'X') {
    paths.push_back(xp + yp);
  }
  if (buttons[ty * 3 + fx] != 'X') {
    paths.push_back(yp + xp);
  }
  return paths;
}

static SPaths
findBasicPaths(const Buttons& buttons) {
  SPaths spaths;
  for (auto f : buttons) {
    for (auto t : buttons) {
      spaths[Edge{f, t}] = findBasicPaths(buttons, f, t);
    }
  }
  return spaths;
}

static Num
len(const ChunkedPath& path) {
  return std::transform_reduce(path.begin(), path.end(), Num{}, std::plus<>(),
      [](auto& i) { return i.first.size() * i.second; });
}

static ChunkedPath
chunked(const Path& path, Num count = 1) {
  ChunkedPath chunks;
  for (std::size_t b = 0, p = path.find('A', b); p != Path::npos; b = p + 1, p = path.find('A', b)) {
    auto chunk = path.substr(b, p + 1 - b);
    chunks[chunk] += count;
  }
  return chunks;
}

static ChunkedPaths
chunked(const Paths& paths, Num count = 1) {
  ChunkedPaths chunkedPaths;
  std::transform(paths.begin(), paths.end(), std::back_inserter<>(chunkedPaths), [count](const auto &p) { return chunked(p, count); });
  return chunkedPaths;
}

static Paths
appendPathsWithA(const Paths& starts, const Paths& ends) {
  Paths paths;
  for (auto start : starts) {
    for (auto end : ends) {
      paths.push_back(start + end + 'A');
    }
  }
  return paths;
}

static ChunkedPaths
appendPaths(const ChunkedPaths& starts, const ChunkedPaths& ends) {
  ChunkedPaths paths;
  for (auto start : starts) {
    for (auto end : ends) {
      ChunkedPath path{start};
      for (auto& [k, v] : end) {
        path[k] += v;
      }
      paths.push_back(path);
    }
  }
  return paths;
}

static ChunkedPaths
nextChunkedPaths(const SPaths& spaths, ChunkedPaths paths) {
  ChunkedPaths nextPaths;
  Button pb = 'A';
  for (const auto& path : paths) {
    ChunkedPaths cps = {ChunkedPath{}};
    for (const auto& [p, count] : path) {
      Paths pp = {Path{}};
      for (Button b : p) {
        pp = appendPathsWithA(pp, spaths.at(Edge{pb, b}));
        pb = b;
      }
      cps = appendPaths(cps, chunked(pp, count));
    }
    std::move(cps.begin(), cps.end(), std::back_inserter<>(nextPaths));
  }
  std::sort(nextPaths.begin(), nextPaths.end(), [](const auto& a, const auto& b) { return len(a) < len(b); });
  auto minLen = len(nextPaths.front());
  nextPaths.erase(std::find_if(nextPaths.begin(), nextPaths.end(), [minLen](const auto& c) { return len(c) > minLen; }), nextPaths.end());
  return nextPaths;
}

static Num
solve(const SPaths apaths, const SPaths bpaths, const std::string& line, int rounds) {
  Button pb = 'A';
  Paths paths = {Path{}};
  for (auto b : line) {
    paths = appendPathsWithA(paths, apaths.at(Edge{pb, b}));
    pb = b;
  }
  auto chunkedPaths = chunked(paths);

  for (int i = 0; i < rounds; ++i) {
    chunkedPaths = nextChunkedPaths(bpaths, std::move(chunkedPaths));
  }

  Num mul = std::stol(line.substr(0, line.length() - 1));
  return len(chunkedPaths.front()) * mul;
}

static Num
solve(const SPaths apaths, const SPaths bpaths, const std::vector<std::string>& lines, int rounds) {
  Num res = 0;
  for (auto line : lines) {
    res += solve(apaths, bpaths, line, rounds);
  }
  return res;
}

/* ------------------------------------------------------------------------ */

int
main() {
  std::vector<std::string> lines;
  std::copy(std::istream_iterator<std::string>(std::cin), std::istream_iterator<std::string>(), std::back_inserter<>(lines));

  auto apaths = findBasicPaths(ABUTTONS);
  auto bpaths = findBasicPaths(BBUTTONS);

  Num res1 = solve(apaths, bpaths, lines, 2);
  std::cout << "1: " << res1 << "\n";

  // Num res2 = solve(apaths, bpaths, lines, 25);
  // std::cout << "2: " << res2 << "\n";
  return 0;
}
