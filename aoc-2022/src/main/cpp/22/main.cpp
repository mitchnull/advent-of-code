#include <iostream>
#include <string>
#include "../utils.h"

using Board = Grid<>;

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
  auto [x, y] = pd.pos;
  if (y == -1 && (50 <= x && x < 100)) {
    return {{0, 150 + x - 50}, {1, 0}};
  }
  return pd; // FIXME
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
score(PosDir pd) {
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

  PosDir pd1 = {findStart(board), {1, 0}};
  // PosDir pd2 = {findStart(board), {1, 0}};

  int numMoves;
  char turnDir;
  while (std::cin) {
    if (std::cin >> numMoves) {
      pd1 = move(board, pd1, numMoves, warpFlat);
      // pd2 = move(board, pd2, numMoves, warpCube);
    }
    if (std::cin >> turnDir) {
      switch (turnDir) {
        case 'L':
          pd1 = { pd1.pos, turnLeft(pd1.dir) };
          // pd2 = { pd2.pos, turnLeft(pd2.dir) };
          break;
        case 'R':
          pd1 = { pd1.pos, turnRight(pd1.dir) };
          // pd2 = { pd2.pos, turnRight(pd2.dir) };
          break;
        default:
          std::cerr << "ERROR: unknown turn: " << turnDir << std::endl;
          exit(1);
      }
    }
  }
  std::cout << "1: " << score(pd1) << std::endl;
  // std::cout << "2: " << score(pd2) << std::endl;

  return 0;
}
