#include <iostream>
#include <cstdint>
#include <vector>


using Num = std::int64_t;
using byte = std::uint8_t;
using Bytes = std::vector<byte>;

struct Machine {
  Num ip = 0, a, b, c;
};

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
run(Machine m, const Bytes& prog) {
  Bytes out;
  while (0 <= m.ip && m.ip < prog.size()) {
    int cmd = prog[m.ip++];
    int op = prog[m.ip++];
    Num combo = std::array<Num, 8> { 0, 1, 2, 3, m.a, m.b, m.c, -1 }[op & 0x7];
    switch (cmd) {
      case 0: // adv
        m.a = m.a / (1 << combo);
        break;
      case 1: // bxl
        m.b = m.b ^ op;
        break;
      case 2: // bst
        m.b = combo & 0x07;
        break;
      case 3: // jnz
        if (m.a) {
          m.ip = op;
        }
        break;
      case 4: // bxc
        m.b = m.b ^ m.c;
        break;
      case 5: { // out
        out.push_back(combo & 0x07);
        break;
      }
      case 6: // bdv
        m.b = m.a / (1 << combo);
        break;
      case 7: // cdv
        m.c = m.a / (1 << combo);
        break;
      default:
        std::cerr << "ABORT\n";
        return {};
    }
  }
  return out;
}

static Num
solve2(Machine m, const Bytes& prog, Num a = 0, int i = 0) {
  if (i >= prog.size()) {
    return a;
  }
  a <<= 3;
  for (int d = !a; d < 8; ++d) {
    m.a = a | d;
    auto out = run(m, prog);
    
    if (out.front() == prog[prog.size() - i - 1]) {
      auto nn = solve2(m, prog, m.a, i + 1);
      if (nn >= 0) {
        return nn;
      }
    }
  }
  return -1;
}

int
main() {
  Machine m;
  Bytes prog;
  std::string str;
  std::cin >> str >> str >> m.a;
  std::cin >> str >> str >> m.b;
  std::cin >> str >> str >> m.c;
  std::cin >> str;
  int n;
  char ch;
  while (std::cin >> n) {
    prog.push_back(n);
    std::cin >> ch;
  }

  auto out = run(m, prog);
  std::cout << out << "\n";

  std::cout << "2: " << solve2(m, prog) << "\n";

  return 0;
}
