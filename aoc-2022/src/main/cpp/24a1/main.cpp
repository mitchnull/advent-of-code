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
#include <queue>
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

using Num = int32;

struct Pos {
  Num x, y;

  friend auto operator<=>(const Pos &a, const Pos &b) = default;

  friend auto operator+(Pos a, const Pos &b) {
    a.x += b.x;
    a.y += b.y;
    return a;
  }
};

constexpr const std::array<Pos, 5> Moves{Pos{1, 0}, Pos{-1, 0}, Pos{0, 1}, Pos{0, -1}, Pos{0, 0}};

enum Tile {
  Up = '^',
  Down = 'v',
  Left = '<',
  Right = '>',
  None = '.',
  Wall = '#',
};

using Tiles = std::vector<Tile>;
using Row = std::vector<Tiles>;
using Map = std::vector<Row>;

static void
print(const Map &map, const Pos &pos) {
  for (uint y = 0; y < map.size(); ++y) {
    const Row &row = map[y];
    for (uint x = 0; x < row.size(); ++x) {
      const Tiles &tiles = row[x];
      if (x == pos.x && y == pos.y) {
        std::cout << 'E';
      } else if (tiles.empty()) {
        std::cout << '.';
      } else if (tiles.size() > 1) {
        std::cout << static_cast<char>('0' + tiles.size());
      } else {
        std::cout << static_cast<char>(tiles.front());
      }
    }
    std::cout << "\n";
  }
}

static Map
advance(const Map &map) {
  uint rows = map.size();
  uint cols = map.front().size();
  Map res(rows, Row(cols, Tiles{}));
  for (uint y = 0; y < rows; ++y) {
    const Row &row = map[y];
    for (uint x = 0; x < cols; ++x) {
      const Tiles &tiles = row[x];
      for (Tile tile : tiles) {
        switch (tile) {
          case Up: res[(y > 1) ? (y - 1) : (rows - 2)][x].push_back(tile); break;
          case Down: res[(y < rows - 2) ? (y + 1) : 1][x].push_back(tile); break;
          case Left: res[y][(x > 1) ? (x - 1) : (cols - 2)].push_back(tile); break;
          case Right: res[y][(x < cols - 2) ? (x + 1) : 1].push_back(tile); break;
          case Wall: res[y][x].push_back(tile);
          case None: break;
        }
      }
    }
  }
  return res;
}

struct State {
  Pos pos;
  uint turn;
};

int
main() {
  string line;
  Map map;
  while (std::getline(std::cin, line) && !line.empty()) {
    Row row;
    for (uint i = 0; i < line.size(); ++i) {
      Tile tile = static_cast<Tile>(line[i]);
      Tiles tiles{};
      if (tile != None) {
        tiles.push_back(tile);
      }
      row.push_back(tiles);
    }
    map.push_back(std::move(row));
  }
  uint rows = map.size();
  uint cols = map.front().size();
  State start{{1, 0}, 1};
  Pos endPos = Pos{static_cast<Num>(cols - 2), static_cast<Num>(rows - 1)};
  print(map, start.pos);

  uint turn = 0;
  std::vector<std::vector<std::vector<uint>>> visited(rows, std::vector<std::vector<uint>>(cols));
  std::queue<State> queue;

  visited[start.pos.y][start.pos.x].push_back(turn);
  queue.push(start);
  while (!queue.empty()) {
    State state = queue.front();
    queue.pop();
    if (state.turn != turn) {
      turn = state.turn;
      map = advance(map);
    }
    uint nextTurn = turn + 1;
    for (const Pos &move : Moves) {
      Pos p = state.pos + move;
      if (p.x < 0 || p.y < 0 || p.x >= cols || p.y >= rows) {
        continue;
      }
      if (!map[p.y][p.x].empty()) {
        continue;
      }
      if (p == endPos) {
        print(map, p);
        std::cout << state.turn << "\n";
        return 0;
      }
      auto &visitedAtPos = visited[p.y][p.x];
      if (std::find(visitedAtPos.cbegin(), visitedAtPos.cend(), nextTurn) != visitedAtPos.end()) {
        continue;
      }
      visitedAtPos.push_back(nextTurn);
      queue.push({p, nextTurn});
    }
  }
  return 0;
}
