#include <algorithm>
#include <bitset>
#include <cctype>
#include <charconv>
#include <deque>
#include <ios>
#include <iostream>
#include <limits>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>

using uint8 = std::uint8_t;
using int8 = std::int8_t;
using uint32 = std::uint32_t;
using int32 = std::int32_t;
using uint64 = std::uint64_t;
using int64 = std::int64_t;

using uint = uint32;

using Stack = std::vector<char>;

static void
print(const std::vector<Stack>& stacks) {
  for (const auto& stack: stacks) {
    std::cout << (stack.empty() ? ' ' : stack.back());
  }
  std::cout << "\n";
}

template <typename C>
static auto
pop_back(C& v) {
  auto res = v.back();
  v.pop_back();
  return res;
}

static void
move(std::vector<Stack>& stacks, uint count, uint from, uint to) {
  while (stacks.size() <= to) {
    stacks.emplace_back();
  }
  while (count-- > 0) {
    stacks[to].push_back(pop_back(stacks[from]));
  }
}

int
main() {
  std::vector<Stack> stacks;
  std::string line;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      break;
    }
    for (uint i =0, j = 1; j < line.size(); ++i, j += 4) {
      char c = line[j];
      if (c < 'A') {
        continue;
      }
      while (stacks.size() <= i) {
        stacks.emplace_back();
      }
      stacks[i].push_back(c);
    }
  }
  for (auto& stack: stacks) {
    std::reverse(stack.begin(), stack.end());
  }
  uint count, from, to;
  std::string word;
  while (std::cin >> word >> count >> word >> from >> word >> to) {
    move(stacks, count, --from, --to);
  }
  print(stacks);
}
