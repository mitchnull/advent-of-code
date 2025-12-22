#include "../utils.h"
#include <unordered_set>
#include <sstream>

using Num = int64_t;
using Dots = std::unordered_set<Pos>;
using Fold = std::pair<char, int>;
using Folds = std::vector<Fold>;


static Dots
fold(const Dots &dots, const Fold &fold) {
  Dots res;
  for (const auto &d : dots) {
    if (fold.first == 'x') {
      if (d.x < fold.second) {
        res.insert(d);
      } else {
        res.emplace(fold.second - (d.x - fold.second), d.y);
      }
    } else {
      if (d.y < fold.second) {
        res.insert(d);
      } else {
        res.emplace(d.x, fold.second - (d.y - fold.second));
      }
    }
  }
  return res;
}

/* ------------------------------------------------------------------------ */

int
main() {
  string line;
  Dots dots;
  while (std::getline(std::cin, line) && !line.empty()) {
    auto ss = std::istringstream{line};
    int x, y;
    char ch;
    ss >> x >> ch >> y;
    dots.emplace(x, y);
  }
  Folds folds;
  while (std::getline(std::cin, line)) {
    auto it = std::find(line.begin(), line.end(), '=');
    folds.emplace_back(*(it - 1), std::stoi(string{it + 1, line.end()}));
  }
  dots = fold(dots, folds.front());
  println("1: {}", dots.size());
  for (auto it = folds.begin() + 1; it < folds.end(); ++it) {
    dots = fold(dots, *it);
  }
  Pos br{};
  for (auto d : dots) {
    br = {std::max(br.x, d.x), std::max(br.y, d.y)};
  }
  auto grid = Grid<>(br.x + 1, br.y + 1, ' ');
  for (auto d : dots) {
    grid[d.x, d.y] = '#';
  }
  println("{}", grid);
  return 0;
}
