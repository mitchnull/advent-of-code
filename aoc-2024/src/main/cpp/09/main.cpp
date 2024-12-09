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

static std::size_t
findFreePos(const std::vector<Block>& blocks, std::size_t e, uint len) {
  for (auto i = 1Z; i < e; ++i) {
    if (blocks[i].id < 0 && blocks[i].len >= len) {
      return i;
    }
  }
  return e;
}

static Num
solve2(std::string_view line) {
  auto blocks = std::vector<Block>();
  for (auto i = 0Z; i < line.size(); ++i) {
    blocks.emplace_back(i % 2 == 0 ? i / 2 : -1, line[i] - '0');
  }

  auto e = blocks.size() - 1;
  while (e > 1) {
    auto block = blocks[e];
    if (block.id < 0) {
      --e;
      continue;
    }
    auto pos = findFreePos(blocks, e, block.len);
    if (pos < e) {
      blocks[e].id = -1;
      auto free = blocks[pos];
      blocks[pos] = block;
      if (free.len > block.len) {
        blocks.emplace(blocks.begin() + pos + 1, -1, free.len - block.len);
      }
    }
    --e;
  }

  auto pos = 0Z;
  Num res = 0;
  for (const auto& b: blocks) {
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
