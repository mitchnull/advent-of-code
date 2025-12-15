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

  friend auto operator<=>(const Pos& a, const Pos& b) = default;

  friend auto operator+(Pos a, const Pos& b) {
    a.x += b.x;
    a.y += b.y;
    return a;
  }
};

constexpr const std::array<Pos, 5> Moves{
  Pos{1, 0},
  Pos{-1, 0},
  Pos{0, 1},
  Pos{0, -1},
  Pos{0, 0}
};

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
print(const Map& map, const Pos& pos) {
  for (uint y = 0; y < map.size(); ++y) {
    const Row& row = map[y];
    for (uint x = 0; x < row.size(); ++x) {
      const Tiles& tiles = row[x];
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

static void
sort(Map& map) {
  for (Row& row: map) {
    for (Tiles& tiles: row) {
      std::sort(tiles.begin(), tiles.end());
    }
  }
}

static Map
advance(const Map& map) {
  uint rows = map.size();
  uint cols = map.front().size();
  Map res(rows, Row(cols, Tiles{}));
  for (uint y = 0; y < rows; ++y) {
    const Row& row = map[y];
    for (uint x = 0; x < cols; ++x) {
      const Tiles& tiles = row[x];
      for (Tile tile: tiles) {
        switch (tile) {
          case Up:
            res[(y > 1) ? (y - 1) : (rows - 2)][x].push_back(tile);
            break;
          case Down:
            res[(y < rows - 2) ? (y + 1) : 1][x].push_back(tile);
            break;
          case Left:
            res[y][(x > 1) ? (x - 1) : (cols - 2)].push_back(tile);
            break;
          case Right:
            res[y][(x < cols - 2) ? (x + 1) : 1].push_back(tile);
            break;
          case Wall:
            res[y][x].push_back(tile);
          case None:
            break;
        }
      }
    }
  }
  sort(res);
  return res;
}

struct MapCache {
  const Map&
  add(Map map) {
    if (!cache.empty() && map == *cache.front()) {
      loop = true;
      last = cache.begin();
      return *cache.front();
    }
    cache.push_back(std::make_unique<Map>(std::move(map)));
    last = cache.end() - 1;
    return *cache.back();
  }

  const Map&
  advance(const Map& map) {
    ++advanceCount;
#if 1
    if (map != **last) {
      std::cerr << "map advance mismatch\n";
      std::abort();
    }
#endif
    if (++last == cache.end()) {
      if (!loop) {
        return add(::advance(map));
      }
      last = cache.begin();
    }
    return **last;
  }

  void debug() {
    std::cout << "advance called " << advanceCount << " times, size: " << cache.size() << "\n";
  }

private:
  std::vector<std::unique_ptr<Map>> cache;
  decltype(cache.cend()) last = cache.cend();
  bool loop = false;
  int advanceCount = 0;
};

struct MapPos {
  const Map* map;
  Pos pos;

  friend auto operator<=>(const MapPos& a, const MapPos& b) = default;
};

struct State {
  MapPos mapPos;
  uint turn;
};

static std::tuple<const Map*, uint>
travel(MapCache& mapCache, const Map* map, const Pos& startPos, const Pos& endPos, uint turn) {
  uint rows = map->size();
  uint cols = map->front().size();
  State start{{map, startPos}, turn + 1};
  print(*map, start.mapPos.pos);

  std::queue<State> queue;
  queue.push(start);
  std::vector<std::vector<std::vector<const Map*>>> visited(rows, std::vector<std::vector<const Map*>>(cols));
  visited[startPos.y][startPos.x].push_back(map);
  while (!queue.empty()) {
    State state = queue.front();
    queue.pop();
    if (state.turn != turn) {
      turn = state.turn;
      map = &mapCache.advance(*map);
    }
    uint nextTurn = turn + 1;
    for (const Pos& move: Moves) {
      Pos p = state.mapPos.pos + move;
      if (p.x < 0 || p.y < 0 || p.x >= cols || p.y >= rows) {
        continue;
      }
      MapPos nextMapPos{map, p};
      if (!(*map)[p.y][p.x].empty()) {
          continue;
      }
      auto& visitedAtPos = visited[p.y][p.x];
      if (std::find(visitedAtPos.cbegin(), visitedAtPos.cend(), map) != visitedAtPos.end()) {
        continue;
      }
      if (p == endPos) {
        print(*map, p);
        mapCache.debug();
        std::cout << state.turn << "\n";
        return {map, turn};
      }
      visitedAtPos.push_back(map);
      queue.push({nextMapPos, nextTurn});
    }
  }
  std::cerr << "No path found after " << turn << " turns\n";
  std::exit(-1);
}

int
main() {
  string line;
  Map initMap;
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
    initMap.push_back(std::move(row));
  }
  sort(initMap);
  uint rows = initMap.size();
  uint cols = initMap.front().size();

  Pos startPos{1, 0};
  Pos endPos = Pos{static_cast<Num>(cols - 2), static_cast<Num>(rows - 1)};
  MapCache mapCache;
  const Map* map = &mapCache.add(std::move(initMap));
  uint turn = 0;
  std::tie(map, turn) = travel(mapCache, map, startPos, endPos, turn);
  std::tie(map, turn) = travel(mapCache, map, endPos, startPos, turn);
  std::tie(map, turn) = travel(mapCache, map, startPos, endPos, turn);
  std::cout << turn << "\n";
  return 0;
}
