#include <algorithm>
#include <bitset>
#include <cctype>
#include <cstdint>
#include <deque>
#include <ios>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <ranges>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using Num = std::uint64_t;
using std::swap;

struct Node {
  std::string key;
  int value;
};

using Bucket = std::vector<Node>;
using HashMap = std::vector<Bucket>;

int
main() {
  HashMap map(256);
  char ch;
  uint8_t hash = 0;
  std::string key;
  while (std::cin >> ch) {
    switch (ch) {
      case ',': {
        key = "";
        hash = 0;
        continue;
      }
      case '-': {
        Bucket& b = map[hash];
        b.erase(std::remove_if(b.begin(), b.end(), [&](auto& n) { return n.key == key; }), b.end());
        continue;
      }
      case '=': {
        std::cin >> ch;
        int value = ch - '0';
        Bucket& b = map[hash];
        auto it = std::find_if(b.begin(), b.end(), [&](auto& n) { return n.key == key; });
        if (it != b.end()) {
          it->value = value;
        } else {
          b.emplace_back(key, value);
        }
        continue;
      }
    }
    key += ch;
    hash += (ch & 0xff);
    hash *= 17;
  }
  Num sum = 0;
  for (uint i = 0, ie = map.size(); i < ie; ++i) {
    const Bucket& b = map[i];
    for (uint j = 0, je = b.size(); j < je; ++j) {
      // std::cout << "### " << b[j].key << ": " << (i + 1) << " * " << j + 1 << " * " << b[j].value << " = " << (i + 1) * (j + 1) * b[j].value << "\n";
      sum += (i + 1) * (j + 1) * b[j].value;
    }
  }
  std::cout << sum << "\n";
  return 0;
}
