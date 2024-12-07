#include <iostream>
#include <cctype>
#include <limits>
#include <deque>
#include <string>
#include <vector>
#include <bitset>

using uint8 = std::uint8_t;
using int8 = std::int8_t;
using uint32 = std::uint32_t;
using int32 = std::int32_t;
using uint64 = std::uint64_t;
using int64 = std::int64_t;

using uint = uint32;
using V = std::vector<std::string>;

enum Commonity { MostCommon, LeastCommon };

uint
getValue(V v, uint p, Commonity comm) {
  uint zeros = 0;
  uint ones = 0;
  for (const auto &s : v) {
    if (s[p] == '0') {
      ++zeros;
    } else if (s[p] == '1') {
      ++ones;
    } else {
      std::cerr << "ERROR: " << s << "\n";
      std::exit(-1);
    }
  }
  char sel;
  switch (comm) {
    case MostCommon:
      sel = '0' + (ones >= zeros);
      break;
    case LeastCommon:
      sel = '0' + (ones < zeros);
      break;
  }
  V vv;
  for (const auto &s : v) {
    if (s[p] == sel) {
      vv.push_back(s);
    }
  }
  if (vv.empty()) {
    std::cerr << "ERROR: empty\n";
    std::exit(-1);
  }
  if (vv.size() == 1) {
    return std::bitset<64>(vv.front()).to_ulong();
  }
  return getValue(vv, ++p, comm);
}

int
main() {
  V v;
  std::string s;
  
  while ((std::cin >> s)) {
    v.push_back(s);
  }
  uint ogr = getValue(v, 0, MostCommon);
  uint csr = getValue(v, 0, LeastCommon);
  std::cout << (ogr * csr) << "\n";
  return 0;
}
