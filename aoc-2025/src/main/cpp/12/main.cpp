#include "../utils.h"
#include <sstream>
#include <iterator>

using Num = int64_t;
using Box = Grid<>;
using Boxes = std::vector<Box>;
using Board = Grid<>;
struct Task {
  Board board;
  std::vector<int> items;
};
using Tasks = std::vector<Task>;

/* ------------------------------------------------------------------------ */

int
main() {
  Num res1{}, res2{};
  std::string line;
  Boxes boxes;
  Tasks tasks;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      continue;
    }
    std::istringstream ss{line};
    int w, h;
    char ch;
    ss >> w >> ch;
    if (ch == ':') {
      std::vector<std::string> lines;
      while (std::getline(std::cin, line) && !line.empty()) {
        lines.push_back(line);
      }
      boxes.push_back(Box(lines, '.'));
    } else {
      ss >> h >> ch;
      std::vector<int> items{std::istream_iterator<int>{ss}, std::istream_iterator<int>()};
      tasks.emplace_back(Board(w, h, '.', '.'), items);
    }
  }

  for (auto b : boxes) {
    println("box:\n{}", b);
  }
  for (auto t: tasks) {
    println("{}x{}: {}", t.board.w(), t.board.h(), t.items);
  }

  println("1: {}", res1);
  println("2: {}", res2);

  return 0;
}
