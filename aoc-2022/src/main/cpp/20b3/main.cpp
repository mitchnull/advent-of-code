#include <algorithm>
#include <bitset>
#include <cctype>
#include <charconv>
#include <deque>
#include <ios>
#include <iostream>
#include <limits>
#include <list>
#include <memory>
#include <numeric>
#include <ranges>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
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

static constexpr const int64 K = 811589153;
static constexpr const uint TIMES = 10;

using List = std::list<int>;
using It = List::iterator;

template <typename T>
static void
print(const std::vector<T> &v) {
  bool first = true;
  std::cout << "{";
  for (const auto &vv : v) {
    if (first) {
      first = false;
    } else {
      std::cout << ", ";
    }
    std::cout << vv;
  }
  std::cout << "}\n";
}

template <typename T>
static void
print(const std::vector<T> nums, const std::vector<uint> idxs) {
  bool first = true;
  std::cout << "{";
  for (auto idx : idxs) {
    if (first) {
      first = false;
    } else {
      std::cout << ", ";
    }
    std::cout << nums[idx];
  }
  std::cout << "}\n";
}

static int64
mod(int64 i, int64 m) {
  return ((i % m) + m) % m;
}

static void
rot(std::vector<It> &nums, List &list, uint i) {
  uint n = nums.size();
  uint m = n - 1;

  It curr = nums[i];
  int64 iv = mod(*curr * K, m) + 1;
  auto it = curr;
  while (iv-- > 0) {
    if (++it == list.end()) {
      it = list.begin();
    }
  }
  list.splice(it, list, curr, std::next(curr));
}

int
main() {
  std::vector<int> nums;
  int num;
  while (std::cin >> num) {
    nums.push_back(num);
  }
  const uint n = nums.size();
  List list;
  std::vector<It> iters;
  iters.reserve(n);
  for (auto num : nums) {
    iters.push_back(list.insert(list.end(), num));
  }
  for (uint t = 0; t < TIMES; ++t) {
    for (uint i = 0; i < n; ++i) {
      rot(iters, list, i);
    }
  }
  uint mzi = 0;
  std::vector<int> mixed;
  mixed.reserve(n);
  for (const auto &num : list) {
    if (num == 0) {
      mzi = mixed.size();
    }
    mixed.push_back(num);
  }
  int64 mres = mixed[mod(mzi + 1000, n)] * K + mixed[mod(mzi + 2000, n)] * K + mixed[mod(mzi + 3000, n)] * K;
  std::cout << mixed[mod(mzi + 1000, n)] * K << " + " << mixed[mod(mzi + 2000, n)] * K << " + "
            << mixed[mod(mzi + 3000, n)] << " = " << mres << "\n";
  return 0;
}
