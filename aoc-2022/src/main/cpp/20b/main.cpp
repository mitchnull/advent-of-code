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
rot(const std::vector<int> &nums, std::vector<uint> &idxs, uint i) {
  uint n = nums.size();
  uint m = n - 1;

  int64 iv = mod(nums[i] * K, m);
  auto it = std::find(idxs.begin(), idxs.end(), i);
  auto ii = it - idxs.begin();
  auto ni = mod(ii + iv, m);
  auto b = idxs.begin();

  if (ni <= ii) {
    std::rotate(b + ni, it, it + 1);
  } else {
    std::rotate(it, it + 1, b + ni + 1);
  }
}

int
main() {
  std::vector<int> nums;
  int num;
  while (std::cin >> num) {
    nums.push_back(num);
  }
  const uint n = nums.size();
  std::vector<uint> idxs;
  idxs.reserve(n);
  for (uint i = 0; i < n; ++i) {
    idxs.push_back(i);
  }

  for (uint t = 0; t < TIMES; ++t) {
    for (uint i = 0; i < n; ++i) {
      rot(nums, idxs, i);
    }
  }
  uint mzi = std::find_if(idxs.begin(), idxs.end(), [&](auto i) { return nums[i] == 0; }) - idxs.cbegin();
  int64 mres =
      nums[idxs[mod(mzi + 1000, n)]] * K + nums[idxs[mod(mzi + 2000, n)]] * K + nums[idxs[mod(mzi + 3000, n)]] * K;
  std::cout << nums[idxs[mod(mzi + 1000, n)]] * K << " + " << nums[idxs[mod(mzi + 2000, n)]] * K << " + "
            << nums[idxs[mod(mzi + 3000, n)]] * K << " = " << mres << "\n";
  return 0;
}
