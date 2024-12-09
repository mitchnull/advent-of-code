#include <iostream>
#include <cstdint>
#include <string>

using Num = std::uint64_t;

struct Block {
  int id;
  uint len;
};

static Num
solve1(std::string_view line) {
  auto disk = std::vector<int>();
  for (auto i = 0Z; i < line.size(); ++i) {
    for (auto j = 0Z; j < line[i] - '0'; ++j) {
      disk.push_back(i % 2 == 0 ? i / 2 : -1);
    }
  }

  auto b = 0Z;
  auto e = disk.size() - 1;

  Num res = 0;
  while (b < e) {
    if (disk[b] >= 0) {
      res += disk[b] * b;
      ++b;
    } else if (disk[e] < 0) {
      --e;
    } else {
      res += disk[e--] * b++;
    }
  }
  return res;
}

// ------------------------------------------------------------------------ //

static Num
solve2(std::string_view line) {
  auto blocks = std::vector<Block>();
  for (auto i = 0Z; i < line.size(); ++i) {
    blocks.emplace_back(i % 2 == 0 ? i / 2 : -1, line[i] - '0');
  }

  for (auto b = blocks.begin(), e = blocks.end() - 1; e != b; --e) {
    if (e->id < 0) {
      continue;
    }
    auto pos = std::find_if(b, e, [&e](auto bl) { return bl.id < 0 && bl.len >= e->len; });
    if (pos != e) {
      std::iter_swap(pos, e);
      if (e->len > pos->len) {
        blocks.emplace(pos + 1, -1, e->len - pos->len);
        (e + 1)->len = pos->len;
      }
    }
  }

  auto pos = 0Z;
  Num res = 0;
  for (auto b: blocks) {
    Num id = (b.id > 0) ?  b.id : 0;
    res += id * (b.len * (2 * pos + b.len - 1) / 2);
    pos += b.len;
  }
  return res;
}

// ------------------------------------------------------------------------ //

int
main() {
  std::string line;
  std::cin >> line;

  std::cout << "1: " << solve1(line) << "\n";
  std::cout << "2: " << solve2(line) << "\n";

  return 0;
}
