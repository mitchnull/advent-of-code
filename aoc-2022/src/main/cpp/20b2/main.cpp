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
#undef SLIST
#ifdef SLIST
#warning "using slist"
#include <boost/intrusive/slist.hpp>
#else
#include <boost/intrusive/list.hpp>
#endif

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

#ifdef SLIST
struct Node: boost::intrusive::slist_base_hook<boost::intrusive::link_mode<boost::intrusive::normal_link>> {
  int value;
  Node(int value) : value(value) {}
};
using List = boost::intrusive::slist<Node, boost::intrusive::constant_time_size<false>>;
#else
struct Node: boost::intrusive::list_base_hook<boost::intrusive::link_mode<boost::intrusive::normal_link>> {
  int value;
  Node(int value) : value(value) {}
};
using List = boost::intrusive::list<Node, boost::intrusive::constant_time_size<false>>;
#endif

template<typename T> 
static void
print(const std::vector<T>& v) {
  bool first = true;
  std::cout << "{";
  for (const auto& vv: v) {
    if (first) {
      first = false;
    } else {
      std::cout << ", ";
    }
    std::cout << vv;
  }
  std::cout << "}\n";
}

template<typename T> 
static void
print(const std::vector<T> nums, const std::vector<uint> idxs) {
  bool first = true;
  std::cout << "{";
  for (auto idx: idxs) {
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
rot(std::vector<Node>& nums, List& nodes, uint i) {
  uint n = nums.size();
  uint m = n - 1;

  Node& num = nums[i];
  int64 iv = mod(num.value * K, m);
  auto it = nodes.erase(nodes.iterator_to(num));
  while (iv-- > 0) {
    if (it == nodes.end()) {
      it = nodes.begin();
    }
    ++it;
  }
  nodes.insert(it, num);
}


int
main() {
  std::vector<Node> nums;
  List nodes;
  int num;
  while (std::cin >> num) {
    nums.push_back(num);
  }
  for (auto it = nums.rbegin(); it != nums.rend(); ++it) {
    nodes.push_front(*it);
  }
  const uint n = nums.size();
  for (uint t = 0; t < TIMES; ++t) {
    for (uint i = 0; i < n; ++i) {
      rot(nums, nodes, i);
    }
  }
  uint mzi = 0;
  std::vector<int> mixed;
  mixed.reserve(n);
  for (const auto& n: nodes) {
    if (n.value == 0) {
      mzi = mixed.size();
    }
    mixed.push_back(n.value);
  }
  int64 mres = mixed[mod(mzi + 1000, n)] * K + mixed[mod(mzi + 2000, n)] * K + mixed[mod(mzi + 3000, n)] * K;
  std::cout << mixed[mod(mzi + 1000, n)] * K << " + " << mixed[mod(mzi + 2000, n)] * K  << " + " << mixed[mod(mzi + 3000, n)] << " = " << mres << "\n";
  return 0;
}
