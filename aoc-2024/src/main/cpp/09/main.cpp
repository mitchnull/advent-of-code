#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

using Num = std::uint64_t;

static Num
solve1(std::string_view line) {
  auto disk = std::vector<int>();
  for (auto i = 0UZ; i < line.size(); ++i) {
    for (auto j = 0UZ; j < line[i] - '0'; ++j) {
      disk.push_back(i % 2 == 0 ? i / 2 : -1);
    }
  }

  Num res = 0;
  for (auto b = 0UZ, e = disk.size() - 1; b < e;) {
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

struct Block {
  int id;
  uint len;
};

static Num
solve2(std::string_view line) {
  auto blocks = std::vector<Block>();
  for (auto i = 0UZ; i < line.size(); ++i) {
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

  auto pos = 0UZ;
  Num res = 0;
  for (auto b : blocks) {
    Num id = (b.id > 0) ? b.id : 0;
    res += id * (b.len * (2 * pos + b.len - 1) / 2);
    pos += b.len;
  }
  return res;
}

// ------------------------------------------------------------------------ //
// Baltazar's version

struct FileBlock {
  int id;
  uint pos;
  uint len;
};

struct FreeBlock {
  uint pos;
  uint len;
};

static Num
solve2b(std::string_view line) {
  auto files = std::vector<FileBlock>();
  auto frees = std::vector<FreeBlock>();
  auto pos = 0U;
  for (auto i = 0UZ; i < line.size(); ++i) {
    auto len = line[i] - '0';
    if (i % 2 == 0) {
      files.emplace_back(i / 2, pos, len);
    } else {
      frees.emplace_back(pos, len);
    }
    pos += len;
  }

  auto firsts = std::vector<decltype(frees.begin())>(10, frees.begin());
  Num res = 0;
  for (auto it = files.rbegin(), rend = files.rend(); it != rend; ++it) {
    auto pos =
        std::find_if(firsts[it->len], frees.end(), [it](auto fb) { return fb.pos >= it->pos || fb.len >= it->len; });
    firsts[it->len] = pos;
    if (pos != frees.end() && pos->pos < it->pos) {
      res += it->id * (it->len * (2 * pos->pos + it->len - 1) / 2);
      pos->len -= it->len;
      pos->pos += it->len;
    } else {
      res += it->id * (it->len * (2 * it->pos + it->len - 1) / 2);
    }
  }
  return res;
}

// ------------------------------------------------------------------------ //

int
main() {
  std::string line;
  std::cin >> line;

  std::cout << "1:  " << solve1(line) << "\n";
  std::cout << "2:  " << solve2(line) << "\n";
  std::cout << "2b: " << solve2b(line) << "\n";

  return 0;
}
