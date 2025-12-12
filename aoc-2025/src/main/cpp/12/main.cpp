#include "../utils.h"
#include <algorithm>
#include <numeric>
#include <sstream>
#include <iterator>

struct Task {
  int w, h;
  std::vector<int> items;
};

static std::pair<int, int>
solve1(const Task& t, const std::vector<int>& blockFills, int ext) {
  const int N = t.items.size();
  int numItems = std::reduce(t.items.begin(), t.items.end(), 0, std::plus<>{});
  int easyPlaces = (t.w / ext) * (t.h / ext);
  if (numItems <= easyPlaces) {
    return {1, 1}; // yep
  }
  int bs = 0;
  for (int i = 0; i < N; ++i) {
    bs += t.items[i] * blockFills[i];
  }
  if (bs > (t.w * t.h)) {
    return {0, 0}; // nope
  }
  return {0, 1}; // maybe...
}

/* ------------------------------------------------------------------------ */


int
main() {
  std::string line;
  std::vector<int> blockFills;
  std::vector<Task> tasks;
  auto ext = 0UZ;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      continue;
    }
    std::istringstream ss{line};
    int w;
    char ch;
    ss >> w >> ch;
    if (ch == ':') {
      auto h = 0UZ;
      int fill = 0;
      while (std::getline(std::cin, line) && !line.empty()) {
        fill += ranges::count(line, '#');
        ext = std::max(ext, line.length());
        ++h;
      }
      ext = std::max(ext, h);
      blockFills.push_back(fill);
    } else {
      int h;
      ss >> h >> ch;
      std::vector<int> items{std::istream_iterator<int>{ss}, std::istream_iterator<int>()};
      tasks.emplace_back(w, h, items);
    }
  }
  auto res1 = std::transform_reduce(tasks.begin(), tasks.end(), std::make_pair(0, 0),
      [](auto a, auto b) { return a + b; },
      [&](const auto& t) { return solve1(t, blockFills, ext); });
  println("1: {}", res1);

  return 0;
}
