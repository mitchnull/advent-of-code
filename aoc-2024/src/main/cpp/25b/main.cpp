#include <iostream>
#include <string>
#include <vector>

using Num = std::uint64_t;
using Nums = std::vector<Num>;

int
main() {
  Nums keys;
  Nums locks;

  std::string line;
  while (std::cin) {
    Num h = 0;
    while (std::getline(std::cin, line) && !line.empty()) {
      for (auto c : line) {
        h = (h <<  1) | (c == '#');
      }
    }
    if (h & 0x01) {
      keys.push_back(h);
    } else {
      locks.push_back(h);
    }
  }

  Num res = 0;
  for (auto& k: keys) {
    for (auto& l: locks) {
      res += !(l & k);
    }
  }

  std::cout << "1: " << res << std::endl;
}
