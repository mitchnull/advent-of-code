#include "../utils.h"
#include <unordered_map>
#include <sstream>
#include <deque>

using Num = int64_t;
using V = std::vector<std::string>;
using Map = std::unordered_map<std::string, V>;

Num
solve1(const Map& m) {
  Num res = 0;
  V p{"you"};
  while (!p.empty()) {
    V next{};
    for (auto from : p) {
      for (auto to : m.find(from)->second) {
        if (to == "out") {
          ++res;
        } else {
          next.push_back(to);
        }
      }
    }
    std::swap(p, next);
  }
  return res;
}

/* ------------------------------------------------------------------------ */

int
main() {
  std::string line;
  Map m;

  while (std::getline(std::cin, line)) {
    std::istringstream ss{line};
    std::string from;
    ss >> from;
    V to{std::istream_iterator<std::string>(ss), std::istream_iterator<std::string>()};
    m[from.substr(0, from.size() - 1)] = to;
  }
  Num res1 = solve1(m);


  println("1: {}", res1);
  println("2: {}", 0);

  return 0;
}
