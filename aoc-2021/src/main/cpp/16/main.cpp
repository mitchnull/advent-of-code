#include "../utils.h"
#include <numeric>

using Num = int64_t;

enum Ops {
  SUM,
  MUL,
  MIN,
  MAX,
  LIT,
  GT,
  LT,
  EQ,
};

struct BitStream {
  string data;
  int p = 0;

  bool read() {
    int ci = p / 4;
    int mask = 1 << (3 - (p++ - ci * 4));
    return std::stoi(string{data[ci]}, nullptr, 16) & mask;
  }

  Num read(int bits) {
    Num res = 0;
    for (int i = 0; i < bits; ++i) {
      res <<= 1;
      res |= read();
    }
    return res;
  }

  Num readLiteral() {
    Num res = 0;
    bool cont;
    do {
      cont = read();
      res <<= 4;
      res += read(4);
    } while (cont);
    return res;
  }
};

static std::pair<Num, Num>
solve(BitStream &bs) {
  auto ver = bs.read(3);
  auto type = bs.read(3);
  if (type == 4) {
    auto lit = bs.readLiteral();
    return {ver, lit};
  } else {
    std::vector<Num> vs;
    bool isNumSubPackets = bs.read();
    auto len = bs.read(isNumSubPackets ? 11 : 15);
    for (Num end = bs.p + len, i = 0; isNumSubPackets ? i < len : bs.p < end; ++i) {
      auto [vsum, v] = solve(bs);
      ver += vsum;
      vs.push_back(v);
    }
    switch (type) {
      case SUM: return {ver, std::reduce(vs.begin(), vs.end(), Num{0}, std::plus<>{})};
      case MUL: return {ver, std::reduce(vs.begin(), vs.end(), Num{1}, std::multiplies<>{})};
      case MIN: return {ver, *std::min_element(vs.begin(), vs.end())};
      case MAX: return {ver, *std::max_element(vs.begin(), vs.end())};
      case GT: return {ver, vs[0] > vs[1]};
      case LT: return {ver, vs[0] < vs[1]};
      case EQ: return {ver, vs[0] == vs[1]};
    }
  }
  return {-1, -1}; // never reached
}

/* ------------------------------------------------------------------------ */

int
main() {
  string data;
  std::cin >> data;

  BitStream bs{data};
  auto [res1, res2] = solve(bs);
  println("1: {}", res1);
  println("2: {}", res2);

  return 0;
}
