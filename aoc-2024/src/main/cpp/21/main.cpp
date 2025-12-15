#include "../utils.h"
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>

using Num = std::uint64_t;
using Button = char;
using Buttons = std::vector<Button>;
using Edge = std::pair<Button, Button>;
using Path = std::string;
using Paths = std::vector<Path>;
using ChunkedPath = std::map<Path, Num>;
using ChunkedPaths = std::vector<ChunkedPath>;
using BasePaths = std::unordered_map<Edge, ChunkedPaths>;

struct CachedChunkedPath {
  ChunkedPath cp;
  int depth;
};

template <>
struct std::hash<Edge> {
  std::size_t operator()(const Edge &e) const { return e.first * 11 + e.second; }
};

template <>
struct std::hash<ChunkedPath> {
  std::size_t operator()(const ChunkedPath &cp) const {
    return std::accumulate(
        cp.begin(), cp.end(), 0, [](auto h, auto &e) { return hashCombine(h, std::hash<Path>{}(e.first) * e.second); });
  }
};

static const auto ABUTTONS = std::vector{
    '7',
    '8',
    '9',
    '4',
    '5',
    '6',
    '1',
    '2',
    '3',
    'X',
    '0',
    'A',
};

static const auto BBUTTONS = std::vector{
    'X',
    '^',
    'A',
    '<',
    'v',
    '>',
};

/* ------------------------------------------------------------------------ */

static std::ostream &
operator<<(auto &os, const ChunkedPath &chunks) {
  if (chunks.empty()) {
    return os << "{}";
  }
  os << "{\n";
  for (auto [k, v] : chunks) {
    os << "  " << k << ": " << v << ",\n";
  }
  return os << "}";
}

static std::ostream &
operator<<(auto &os, const Paths &paths) {
  if (paths.empty()) {
    return os << "{}";
  }
  os << "{\n";
  for (auto p : paths) {
    os << "  " << p << ",\n";
  }
  return os << "}";
}

static std::ostream &
operator<<(auto &os, const ChunkedPaths &chunkedPaths) {
  if (chunkedPaths.empty()) {
    return os << "[]";
  }
  os << "[\n";
  for (auto &p : chunkedPaths) {
    os << "  " << p << ",\n";
  }
  return os << "]";
}

static std::ostream &
operator<<(auto &os, const BasePaths &basePaths) {
  if (basePaths.empty()) {
    return os << "{}";
  }
  os << "{\n";
  for (auto &[e, cp] : basePaths) {
    os << "  " << e.first << e.second << ": " << cp << ",\n";
  }
  return os << "}";
}

/* ------------------------------------------------------------------------ */

static ChunkedPath
chunked(const Path &path, Num count = 1) {
  ChunkedPath chunks;
  for (std::size_t b = 0, p = path.find('A', b); p != Path::npos; b = p + 1, p = path.find('A', b)) {
    auto chunk = path.substr(b, p + 1 - b);
    chunks[chunk] += count;
  }
  return chunks;
}

static Paths
findBasePaths(const Buttons &buttons, Button f, Button t) {
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

static BasePaths
findBasePaths(const Buttons &buttons) {
  BasePaths basePaths;
  for (auto f : buttons) {
    for (auto t : buttons) {
      Paths paths = findBasePaths(buttons, f, t);
      ChunkedPaths chunkedPaths;
      std::transform(paths.begin(), paths.end(), std::back_inserter<>(chunkedPaths), [](const auto &p) {
        return chunked(p + 'A');
      });
      basePaths[{f, t}] = chunkedPaths;
    }
  }
  return basePaths;
}

static Num
len(const ChunkedPath &path) {
  return std::transform_reduce(
      path.begin(), path.end(), Num{}, std::plus<>(), [](auto &i) { return i.first.size() * i.second; });
}

static ChunkedPath
nextChunkedPath(const BasePaths &basePaths, const ChunkedPath &chunkedPath, int depth) {
  static std::unordered_map<ChunkedPath, CachedChunkedPath> cache;
  ChunkedPath res;
  Button pb = 'A';
  for (const auto &[p, count] : chunkedPath) {
    for (Button b : p) {
      const auto &bps = basePaths.at({pb, b});
      auto sel = bps.begin();
      if (bps.size() > 1 && depth > 0) {
        ChunkedPaths expandedPaths = bps;
        for (int d = depth; d > 0; --d) {
          std::transform(expandedPaths.begin(), expandedPaths.end(), expandedPaths.begin(), [&](auto &cp) {
            if (auto it = cache.find(cp); it != cache.end() && it->second.depth >= depth) {
              return it->second.cp;
            }
            return (cache[cp] = {nextChunkedPath(basePaths, cp, d - 1), depth}).cp;
          });
          auto [itMin, itMax] = std::minmax_element(
              expandedPaths.begin(), expandedPaths.end(), [](auto &a, auto &b) { return len(a) < len(b); });
          if (len(*itMin) != len(*itMax)) {
            sel += itMin - expandedPaths.begin();
            break;
          }
        }
      }
      for (auto &[k, v] : *sel) {
        res[k] += v * count;
      }
      pb = b;
    }
  }
  return res;
}

static Num
solve(const BasePaths basePaths, const std::string &line, int rounds) {
  auto chunkedPath = chunked(line);
  for (int i = 0; i <= rounds; ++i) {
    chunkedPath = nextChunkedPath(basePaths, chunkedPath, rounds - i);
  }

  Num mul = std::stol(line.substr(0, line.length() - 1));
  return len(chunkedPath) * mul;
}

static Num
solve(const BasePaths basePaths, const std::vector<std::string> &lines, int rounds) {
  Num res = 0;
  for (auto line : lines) {
    res += solve(basePaths, line, rounds);
  }
  return res;
}

/* ------------------------------------------------------------------------ */

int
main() {
  std::vector<std::string> lines;
  std::copy(
      std::istream_iterator<std::string>(std::cin), std::istream_iterator<std::string>(), std::back_inserter<>(lines));

  auto basePaths = findBasePaths(ABUTTONS);
  basePaths.merge(findBasePaths(BBUTTONS));

  Num res1 = solve(basePaths, lines, 2);
  std::cout << "1: " << res1 << "\n";

  Num res2 = solve(basePaths, lines, 25);
  std::cout << "2: " << res2 << "\n";
}
