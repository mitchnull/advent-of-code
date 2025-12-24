#include "../utils.h"

using Num = int64_t;

struct BitStream {
  string data;
  int p = 0;

  bool read() {
    int ci = p / 4;
    int mask = 1 << (3 - (p++ - ci * 4));
    return std::stoi(string{data[ci]}, nullptr, 16) & mask;
  }

  int read(int bits) {
    int res = 0;
    for (int i = 0; i < bits; ++i) {
      res <<= 1;
      res |= read();
    }
    return res;
  }

  int readLiteral() {
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

static Num
solve1(BitStream &bs) {
  Num res = 0;
  auto v = bs.read(3);
  res += v;
  auto t = bs.read(3);
  if (t == 4) {
    auto lit = bs.readLiteral();
    println("@@@ v: {}, t: {}, lit: {}", v, t, lit);
  } else {
    bool isNumSubPackets = bs.read();
    if (isNumSubPackets) {
      auto numSubPackets = bs.read(11);
      println("@@@ v: {}, t: {}, numSub: {}", v, t, numSubPackets);
      for (int i = 0; i < numSubPackets; ++i) {
        res += solve1(bs);
      }
    } else {
      auto subLen = bs.read(15);
      println("@@@ v: {}, t: {}, subLen: {}", v, t, subLen);
      for (auto end = bs.p + subLen; bs.p < end;) {
        res += solve1(bs);
      }
    }
  }
  return res;
}

/* ------------------------------------------------------------------------ */

int
main() {
  string data;
  std::cin >> data;

  BitStream bs{data};
  println("1: {}", solve1(bs));

  return 0;
}
