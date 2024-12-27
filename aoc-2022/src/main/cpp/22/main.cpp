#include <iostream>
#include <string>
#include <sstream>
#include "../utils.h"

using Board = Grid<>;

static constexpr const int BS = 50;
// static constexpr const int BS = 5;

static const auto DIRSL = std::vector<Dir> {
  {1, 0},
  {0, 1},
  {-1, 0},
  {0, -1},
};

struct PosDir {
  Pos pos;
  Dir dir;

  friend auto operator<=>(const PosDir&, const PosDir&) = default;
};

/* ------------------------------------------------------------------------ */

static Dir
turnLeft(Dir d) {
  std::cout << "@@@ turning left: " << Dir{d.dy, -d.dx} << std::endl;
  return {d.dy, -d.dx};
}

static Dir
turnRight(Dir d) {
  std::cout << "@@@ turning right" << Dir{-d.dy, d.dx} << std::endl;
  return {-d.dy, d.dx};
}

static PosDir
warpFlat(const Board& board, PosDir pd) {
  pd.pos.x = (pd.pos.x + board.w()) % board.w();
  pd.pos.y = (pd.pos.y + board.h()) % board.h();
  while (board[pd.pos] == ' ') {
    pd.pos += pd.dir;
    pd.pos.x = (pd.pos.x + board.w()) % board.w();
    pd.pos.y = (pd.pos.y + board.h()) % board.h();
  }
  return pd;
}

static PosDir
warpCube(const Board& board, PosDir pd) {
  //  21
  //  3
  // 54
  // 6
  auto [x, y] = pd.pos;
  if (y == -1 && BS <= x && x < 2 * BS) { /* 2U -> 6L */
    return {{0, 3 * BS + x - BS}, {1, 0}};
  }
  if (y == -1 && 2 * BS <= x && x < 3 * BS) { /* 1U -> 6D */
    return {{x - 2 * BS, 4 * BS - 1}, {0, -1}};
  }
  if (x == 3 * BS && 0 <= y && y < BS) { /* 1R -> -4R */
    return {{2 * BS - 1, 3 * BS - 1 - y}, {-1, 0}};
  }
  if (y == BS && 2 * BS <= x && x < 3 * BS) { /* 1D -> 3R */
    return {{2 * BS - 1, BS + x - 2 * BS}, {-1, 0}};
  }
  if (x == 2 * BS && BS <= y && y < 2 * BS) { /* 3R -> 1D */
    return {{2 * BS + y - BS, BS - 1}, {0, -1}};
  }
  if (x == 2 * BS && 2 * BS <= y && y < 3 * BS) { /* 4R -> -1R */
    return {{3 * BS - 1, BS - 1 - (y - 2 * BS)}, {-1, 0}};
  }
  if (y == 3 * BS && BS <= x && x < 2 * BS) { /* 4D -> 6R */
    return {{BS - 1, 3 * BS + x - BS}, {-1, 0}};
  }
  if (x == BS && 3 * BS <= y && y < 4 * BS) { /* 6R -> 4D */
    return {{BS + y - 3 * BS, 3 * BS - 1}, {0, -1}};
  }
  if (y == 4 * BS && 0 <= x && x < BS) { /* 6D -> 1U */
    return {{2 * BS + x, 0}, {0, 1}};
  }
  if (x == -1 && 3 * BS <= y && y <= 4 * BS) { /* 6L -> 2U */
    return {{BS + y - 3 * BS, 0}, {0, 1}};
  }
  if (x == -1 && 2 * BS <= y && y < 3 * BS) { /* 5L -> -2L */
    return {{BS, BS - 1 - (y - 2 * BS)}, {1, 0}};
  }
  if (y == 2 * BS - 1 && 0 <= x && x < BS) { /* 5U -> 3L */
    return {{BS, BS + x}, {1, 0}};
  }
  if (x == BS - 1 && BS <= y && y < 2 * BS) { /* 3L -> 5U */
    return {{y - BS, 2 * BS}, {0, 1}};
  }
  if (x == BS - 1 && 0 <= y && y < BS) { /* 2L -> -5L */
    return {{0, 3 * BS - 1 - y}, {1, 0}};
  }
  return pd;
}

static PosDir
move(const Board& board, PosDir pd, int numMoves, auto warp) {
  std::cout << "@@@ moving from " << pd.pos << pd.dir << ": " << numMoves << std::endl;
  auto warpWithLog = [warp](const Board& board, PosDir pd) {
    auto res = warp(board, pd);
    if (res != pd) {
      std::cout << "@@@ warped from " << pd.pos << pd.dir << " to " << res.pos << res.dir << std::endl;
    }
    return res;
  };
  while (numMoves-- > 0) {
    auto npd = warpWithLog(board, {pd.pos + pd.dir, pd.dir});
    if (board[npd.pos] != '.') {
      std::cout << "@@@ hit obstacle at" << npd.pos << std::endl;
      break;
    }
    pd = npd;
  }
  std::cout << "@@@ moved to " << pd.pos << pd.dir << std::endl;
  return pd;
}

static Pos
findStart(const Board& board) {
  for (auto [x, y, v]: board.iter()) {
    if (v == '.') {
      return {x, y};
    }
  }
  return {};
}

static int
solve(const Board& board, const std::string& steps, auto warp) {
  PosDir pd = {findStart(board), {1, 0}};

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

static void
assertEquals(std::string name, PosDir a, PosDir e) {
  if (a != e) {
    std::cerr << "FAIL: " << name << ": " << a.pos << a.dir << " != " << e.pos << e.dir << std::endl;
  }
}

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


  // auto res1 = solve(board, steps, warpFlat);
  auto res2 = solve(board, steps, warpCube);
  // std::cout << "1: " << res1 << std::endl;
  std::cout << "2: " << res2 << std::endl;

  assertEquals("2U -> 6L", warpCube(board, {{51, -1}, {0, -1}}), {{0, 151}, {1, 0}});
  assertEquals("1U -> 6D", warpCube(board, {{102, -1}, {0, -1}}), {{2, 199}, {0, -1}});
  assertEquals("1R -> -4R", warpCube(board, {{150, 1}, {1, 0}}), {{99, 148}, {-1, 0}});
  assertEquals("1D -> 3R", warpCube(board, {{102, 50}, {0, 1}}), {{99, 52}, {-1, 0}});
  assertEquals("3R -> 1D", warpCube(board, {{100, 52}, {1, 0}}), {{102, 49}, {0, -1}});
  assertEquals("4R -> -1R", warpCube(board, {{100, 101}, {1, 0}}), {{149, 48}, {-1, 0}});
  assertEquals("4D -> 6R", warpCube(board, {{51, 150}, {0, 1}}), {{49, 151}, {-1, 0}});
  assertEquals("6R -> 4D", warpCube(board, {{50, 151}, {1, 0}}), {{51, 149}, {0, -1}});
  assertEquals("6D -> 1U", warpCube(board, {{1, 200}, {0, 1}}), {{101, 0}, {0, 1}});
  assertEquals("6L -> 2U", warpCube(board, {{-1, 151}, {-1, 0}}), {{51, 0}, {0, 1}});
  assertEquals("5L -> -2L", warpCube(board, {{-1, 101}, {-1, 0}}), {{50, 48}, {1, 0}});
  assertEquals("5U -> 3L", warpCube(board, {{1, 99}, {0, -1}}), {{50, 51}, {1, 0}});
  assertEquals("3L -> 5U", warpCube(board, {{49, 51}, {-1, 0}}), {{1, 100}, {0, 1}});
  assertEquals("2L -> -5L", warpCube(board, {{49, 1}, {-1, 0}}), {{0, 148}, {1, 0}});
  return 0;
}
