#include <algorithm>
#include <bitset>
#include <cctype>
#include <charconv>
#include <deque>
#include <ios>
#include <iostream>
#include <limits>
#include <memory>
#include <numeric>
#include <ranges>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

using uint8 = std::uint8_t;
using int8 = std::int8_t;
using uint32 = std::uint32_t;
using int32 = std::int32_t;
using uint64 = std::uint64_t;
using int64 = std::int64_t;

using uint = uint32;
using string = std::string;
using namespace std::literals;
using size_t = std::size_t;

using Board = std::vector<string>;

enum Facing {
  Right,
  Down,
  Left,
  Up,

  NumFacings
};

struct Pos {
  size_t row, col;
  Facing facing;

  Pos&
  turn(int dir) {
    facing = static_cast<Facing>((facing + NumFacings + dir) % NumFacings);
    return *this;
  }
};

enum Op {
  Move,
  TurnLeft = 'L',
  TurnRight = 'R',
};

struct Step {
  Op op = Move;
  uint steps = 0;
  size_t sp = 0;

  bool
  isLast() const {
    return op == Move && steps == 0;
  }
};

using It = std::string::const_iterator;

Step
readStep(const string& str, size_t pos) {
  if (pos >= str.size()) {
    return {Move, 0, pos};
  }
  switch (str[pos]) {
    case 'L':
      return {TurnLeft, 0, ++pos};
    case 'R':
      return {TurnRight, 0, ++pos};
  }
  uint steps = 0;
  while (pos < str.size() && std::isdigit(str[pos])) {
    steps *= 10;
    steps += str[pos++] - '0';
  }
  return {Move, steps, pos};
}

static Facing 
rev(Facing facing) {
  switch (facing) {
    case Left:
      return Right;
    case Right:
      return Left;
    case Up:
      return Down;
    case Down:
      return Up;
    case NumFacings:
      break;
  }
  return facing; // never reached
}

static void
moveOne(const Board& board, Pos& pos, Facing direction) {
  switch (direction) {
    case Left:
      do {
        pos.col = pos.col == 0 ? board[pos.row].size() - 1 : pos.col - 1;
      } while (board[pos.row][pos.col] == ' ');
      break;
    case Right:
      do {
        if (++pos.col >= board[pos.row].size()) {
          pos.col = 0;
        }
      } while (board[pos.row][pos.col] == ' ');
      break;
    case Up:
      do {
        pos.row = pos.row == 0 ? board.size() - 1 : pos.row - 1;
      } while (board[pos.row].size() <= pos.col ||
            board[pos.row][pos.col] == ' ');
      break;
    case Down:
      do {
        if (++pos.row >= board.size()) {
          pos.row = 0;
        }
      } while (board[pos.row].size() <= pos.col ||
            board[pos.row][pos.col] == ' ');
      break;
    case NumFacings:
      break; // never reached
  }
}

static void
moveOne(const Board& board, Pos& pos) {
  moveOne(board, pos, pos.facing);
}

static void
moveBackOne(const Board& board, Pos& pos) {
  moveOne(board, pos, rev(pos.facing));
}

static char
facingSign(Facing facing) {
  switch (facing) {
    case Left:
      return '<';
    case Right:
      return '>';
    case Up:
      return '^';
    case Down:
      return 'V';
    case NumFacings:
      break;
  }
  return '?';
}

static void
print(const Board& board, const Pos& pos) {
  for (uint row = 0; row < board.size(); ++row) {
    if (pos.row == row) {
      string line = board[row];
      line[pos.col] = facingSign(pos.facing);
      std::cout << line << "\n";
    } else {
      std::cout << board[row] << "\n";
    }
  }
  std::cout << "\n";
}

static void
move(const Board& board, Pos& pos, uint steps) {
  while (steps-- > 0) {
    moveOne(board, pos);
    if (board[pos.row][pos.col] == '#') {
      moveBackOne(board, pos);
      break;
    }
  }
}

int
main() {
  Board board;

  string line;
  while (std::getline(std::cin, line) && !line.empty()) {
    board.push_back(std::move(line));
  }
  std::getline(std::cin, line);
  Pos pos{0, board.front().find_first_of('.', 0), Right};
  // print(board, pos);
  Step step{};
  while (!(step = readStep(line, step.sp)).isLast()) {
    switch (step.op) {
      case TurnLeft:
        // std::cout << "@ TurnLeft:\n";
        pos.turn(-1);
        break;
      case TurnRight:
        // std::cout << "@ TurnRight:\n";
        pos.turn(+1);
        break;
      case Move:
        // std::cout << "@ Move " << step.steps << ":\n";
        move(board, pos, step.steps);
        break;
    }
    // print(board, pos);
  }
  std::cout << 1000 * ++pos.row + 4 * ++pos.col + pos.facing << "\n";
  return 0;
}
