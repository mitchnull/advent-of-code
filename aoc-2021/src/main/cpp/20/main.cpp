#include "../utils.h"
#include <algorithm>

using Num = int64_t;
using Image = Grid<>;

static Num
solve(Image image, const string &mapping, int count) {
  for (int i = 0; i < count; ++i) {
    Image next = Image(image.w() + 2, image.h() + 2, {}, image[-1, -1] == '.' ? mapping.front() : mapping.back());
    for (int y = 0; y < next.h(); ++y) {
      for (int x = 0; x < next.w(); ++x) {
        int bin = 0;
        for (int dy = -1; dy <= 1; ++dy) {
          for (int dx = -1; dx <= 1; ++dx) {
            bin = (bin << 1) + (image[x + dx - 1, y + dy - 1] == '#');
          }
        }
        next[x, y] = mapping[bin];
      }
    }
    image = std::move(next);
  }
  return ranges::count(image, '#');
}

/* ------------------------------------------------------------------------ */

int
main() {
  string mapping;
  string line;
  while (std::getline(std::cin, line) && !line.empty()) {
    mapping += line;
  }
  Image image = Image::read(std::cin, '.');

  println("1: {}", solve(image, mapping, 2));
  println("2: {}", solve(image, mapping, 50));

  return 0;
}
