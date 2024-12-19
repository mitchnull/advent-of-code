#include <cstdint>
#include <iostream>
#include <vector>


using Num = std::int64_t;
using byte = std::uint8_t;
using Bytes = std::vector<byte>;

static std::ostream&
operator<<(std::ostream& os, Bytes bytes) {
  std::string sep;
  for (auto b : bytes) {
    os << sep << +b;
    sep = ",";
  }
  return os;
}

Bytes
run(const Bytes& prog, Num a, Num b, Num c, int ip = 0) {
  Bytes out;
  while (0 <= ip && ip < prog.size()) {
    int cmd = prog[ip++];
    int op = prog[ip++];
    Num combo = std::array<Num, 8> { 0, 1, 2, 3, a, b, c, -1 }[op & 0x7];
    switch (cmd) {
      case 0: // adv
        a = a / (1 << combo);
        break;
      case 1: // bxl
        b = b ^ op;
        break;
      case 2: // bst
        b = combo & 0x07;
        break;
      case 3: // jnz
        if (a) {
          ip = op;
        }
        break;
      case 4: // bxc
        b = b ^ c;
        break;
      case 5: { // out
        out.push_back(combo & 0x07);
        break;
      }
      case 6: // bdv
        b = a / (1 << combo);
        break;
      case 7: // cdv
        c = a / (1 << combo);
        break;
      default:
        std::cerr << "ABORT\n";
        return {};
    }
  }
  return out;
}

static Num
solve2(const Bytes& prog, Num a, Num b, Num c, int i = 0) {
  if (i >= prog.size()) {
    return a;
  }
  a <<= 3;
  for (Num aa = a + !a; aa < a + 8; ++aa) {
    auto out = run(prog, aa, b, c);
    
    if (out.front() == prog[prog.size() - i - 1]) {
      auto nn = solve2(prog, aa, b, c, i + 1);
      if (nn >= 0) {
        return nn;
      }
    }
  }
  return -1;
}

int
main() {
  Num a, b, c;
  Bytes prog;
  std::string str;
  std::cin >> str >> str >> a;
  std::cin >> str >> str >> b;
  std::cin >> str >> str >> c;
  std::cin >> str;
  int n;
  char ch;
  while (std::cin >> n) {
    prog.push_back(n);
    std::cin >> ch;
  }

  auto out = run(prog, a, b, c);
  std::cout << out << "\n";

  std::cout << "2: " << solve2(prog, 0, b, c) << "\n";

  return 0;
}
