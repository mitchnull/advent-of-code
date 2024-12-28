#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include "../utils.h"

using Board = Grid<>;

static constexpr const auto DIRSL = std::array<Dir, 4>{
  Dir{1, 0},
  Dir{0, 1},
  Dir{-1, 0},
  Dir{0, -1},
};

using CubeConf = std::string;

struct WarpLine {
  Pos srcBegin, srcEnd;
  Dir srcDir;
  Pos dstBegin, dstEnd;
  Dir dstDir;
};

struct PosDir {
  Pos pos;
  Dir dir;

  friend auto operator<=>(const PosDir&, const PosDir&) = default;
};

struct CubeVertex {
  Pos pos;
  char name;

  friend auto operator<=>(const CubeVertex& a, const CubeVertex& b) { return a.name <=> b.name; }
};

struct CubeEdge {
  CubeVertex a, b;
  Dir dir;

  std::string id() { return {a.name, b.name}; };

  static CubeEdge ordered(CubeVertex a, CubeVertex b, Dir dir) {
    return CubeEdge{std::min(a, b), std::max(a, b), dir};
  }
};

/* ------------------------------------------------------------------------ */

static Dir
turnLeft(Dir d) {
  return {d.dy, -d.dx};
}

static Dir
turnRight(Dir d) {
  return {-d.dy, d.dx};
}

static PosDir
warpAroundBoard(const Board& board, PosDir pd) {
  pd.pos.x = (pd.pos.x + board.w()) % board.w();
  pd.pos.y = (pd.pos.y + board.h()) % board.h();
  while (board[pd.pos] == ' ') {
    pd.pos += pd.dir;
    pd.pos.x = (pd.pos.x + board.w()) % board.w();
    pd.pos.y = (pd.pos.y + board.h()) % board.h();
  }
  return pd;
}

static int
hvLen(Dir d) {
  return std::max(std::abs(d.dx), std::abs(d.dy));
}

static PosDir
warpAtLines(const std::vector<WarpLine>& warpLines, PosDir pd) {
  for (auto& wl : warpLines) {
    if (pd.dir == wl.srcDir &&
        std::min(wl.srcBegin.x, wl.srcEnd.x) <= pd.pos.x && pd.pos.x <= std::max(wl.srcBegin.x, wl.srcEnd.x) &&
        std::min(wl.srcBegin.y, wl.srcEnd.y) <= pd.pos.y && pd.pos.y <= std::max(wl.srcBegin.y, wl.srcEnd.y)) {
      return {wl.dstBegin + ((wl.dstEnd - wl.dstBegin) / (hvLen(wl.dstEnd - wl.dstBegin))) * hvLen(pd.pos - wl.srcBegin), wl.dstDir};
    }
  }
  return pd;
}

static PosDir
move(const Board& board, PosDir pd, int numMoves, auto warp) {
  while (numMoves-- > 0) {
    auto npd = warp({pd.pos + pd.dir, pd.dir});
    if (board[npd.pos] != '.') {
      break;
    }
    pd = npd;
  }
  return pd;
}

static Pos
findStart(const Board& board, auto pred) {
  for (auto [x, y, v]: board.iter()) {
    if (pred(v)) {
      return {x, y};
    }
  }
  return {};
}

static void
findWarpLines(std::vector<WarpLine>& warpLines, const Board& board, int bs, Pos pos, CubeConf cubeConf, std::unordered_map<std::string, CubeEdge>& edges, std::unordered_set<Pos>& visited) {
  static constexpr const auto cubeRotations = std::array<CubeConf, 4>{
    // match order with DIRSL
    "EFBAHGCD",
    "BFGCAEHD",
    "DCGHABFE",
    "EADHFBCG",
  };
  const auto corners = std::array<Dir, 4>{
    Dir{bs - 1, 0},
    Dir{bs - 1, bs - 1},
    Dir{0, bs - 1},
    Dir{0, 0},
  };

  visited.insert(pos);
  for (int i = 0; i < 4; ++i) {
    auto ei = (i + 1) % 4;
    CubeEdge edge = CubeEdge::ordered({pos + corners[i], cubeConf[i]}, {pos + corners[ei], cubeConf[ei]}, DIRSL[i]);
    if (auto it = edges.find(edge.id()); it != edges.end()) {
      warpLines.emplace_back(it->second.a.pos + it->second.dir, it->second.b.pos + it->second.dir, it->second.dir, edge.a.pos, edge.b.pos, -edge.dir);
      warpLines.emplace_back(edge.a.pos + edge.dir, edge.b.pos + edge.dir, edge.dir, it->second.a.pos, it->second.b.pos, -it->second.dir);
    } else {
      edges[edge.id()] = edge;
    }
  }

  for (int i = 0; i < 4; ++i) {
    auto pp = pos + (bs * DIRSL[i]);
    if (board[pp] != ' ' && !visited.contains(pp)) {
      CubeConf rotated = cubeRotations[i]
        | views::transform([&](auto c) { return cubeConf[c - 'A']; })
        | std::ranges::to<CubeConf>();
      findWarpLines(warpLines, board, bs, pp, rotated, edges, visited);
    }
  }
}

static std::vector<WarpLine>
findWarpLines(const Board& board, int bs) {
  std::vector<WarpLine> warpLines;
  Pos pos = findStart(board, [](char c) { return c != ' '; });
  std::unordered_set<Pos> visited;
  std::unordered_map<std::string, CubeEdge> cubeEdges;
  findWarpLines(warpLines, board, bs, pos, "ABCDEFGH", cubeEdges, visited);
  return warpLines;
}

static int
solve(const Board& board, const std::string& steps, auto warp) {
  PosDir pd = {findStart(board, [](char c) { return c == '.'; }), {1, 0}};

  auto ss = std::stringstream(steps);
  int numMoves;
  char turnDir;
  while (ss) {
    if (ss >> numMoves) {
      pd = move(board, pd, numMoves, warp);
    }
    if (ss >> turnDir) {
      switch (turnDir) {
        case 'L':
          pd = { pd.pos, turnLeft(pd.dir) };
          break;
        case 'R':
          pd = { pd.pos, turnRight(pd.dir) };
          break;
        default:
          std::cerr << "ERROR: unknown turn: " << turnDir << std::endl;
          exit(1);
      }
    }
  }
  return 1000 * (pd.pos.y + 1) + 4 * (pd.pos.x + 1) + (std::find(DIRSL.begin(), DIRSL.end(), pd.dir) - DIRSL.begin());
}

/* ------------------------------------------------------------------------ */

int
main() {
  std::vector<std::string> lines;
  std::string line;
  while (std::getline(std::cin, line) && !line.empty()) {
    lines.push_back(line);
  }
  auto len = std::max_element(lines.begin(), lines.end(),
      [](auto a, auto b) { return a.size() < b.size(); })->size();
  for (auto& line : lines) {
    line.resize(len, ' ');
  }
  auto board = Board(lines, ' ');
  std::string steps;
  std::cin >> steps;

  int bs = std::sqrt((std::ranges::count_if(board, [](char c) { return c != ' '; }) / 6));
  auto warpLines = findWarpLines(board, bs);

  auto warp1 = [&board](PosDir pd) { return warpAroundBoard(board, pd); };
  auto warp2 = [&warpLines](PosDir pd) { return warpAtLines(warpLines, pd); };

  auto res1 = solve(board, steps, warp1);
  auto res2 = solve(board, steps, warp2);
  std::cout << "1: " << res1 << std::endl;
  std::cout << "2: " << res2 << std::endl;
}
