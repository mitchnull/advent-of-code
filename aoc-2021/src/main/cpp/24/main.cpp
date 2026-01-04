#include "../utils.h"
#include <sstream>
#include <variant>

using Num = int64_t;

struct Block {
  int div, dx, dy;
};

template <>
struct std::formatter<Block> {
  template <typename FormatContext>
  constexpr auto parse(FormatContext &ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  constexpr auto format(const Block &b, FormatContext &ctx) const {
    return format_to(ctx.out(), "{{div={}, dx={}, dy={}}}", b.div, b.dx, b.dy);
  }
};

using Blocks = std::vector<Block>;

enum Mnemoic {
  INP,
  ADD,
  MUL,
  DIV,
  MOD,
  EQL,
};

static string
name(Mnemoic m) {
  switch (m) {
    case INP: return "inp";
    case ADD: return "add";
    case MUL: return "mul";
    case DIV: return "div";
    case MOD: return "mod";
    case EQL: return "eql";
  }
}

using Op = std::variant<char, Num>;

static string
to_string(Op op) {
  return std::holds_alternative<Num>(op) ? std::to_string(std::get<Num>(op)) : string{std::get<char>(op)};
}

struct Inst {
  Mnemoic m;
  char op1;
  Op op2;
};

using Prog = std::vector<Inst>;
using Regs = std::vector<Num>;

static Op
op(string v) {
  return std::isalpha(v[0]) ? Op(v[0]) : std::stol(v);
}

static Num
v(const Regs &r, Op op) {
  if (std::holds_alternative<Num>(op)) {
    return std::get<Num>(op);
  }
  return r[std::get<char>(op)];
}

static bool
eval(const Prog &prog, Num n) {
  auto r = Regs(256, 0);
  string input = std::to_string(n);
  if (std::find(begin(input), end(input), '0') != end(input)) {
    return false;
  }
  auto it = input.begin();
  for (const auto &i : prog) {
    switch (i.m) {
      case INP: r[i.op1] = (*it++ - '0'); break;
      case ADD: r[i.op1] += v(r, i.op2); break;
      case MUL: r[i.op1] *= v(r, i.op2); break;
      case DIV: r[i.op1] /= v(r, i.op2); break;
      case MOD: r[i.op1] %= v(r, i.op2); break;
      case EQL: r[i.op1] = (r[i.op1] == v(r, i.op2)); break;
    }
  }
  return r['z'] == 0;
}

static Num
solve1(const Blocks &blocks, Num n = 0, int z = 0, int i = 0) {
  if (i >= blocks.size()) {
    // println("@@@ n={}, z={}", n, z);
    return z == 0 ? n : -1;
  }
  auto [div, dx, dy] = blocks[i];
  for (int d = 9; d > 0; --d) {
    if (div == 26 && (z % 26 + dx) != d) {
      // println("@@@ n={}, z={}, div={}, d={}, dx={}, (z % 26 +dx)={}", n, z, div, d, dx, (z % 26 + dx));
      continue;
    }
    int nz = (div == 1) ? (z * 26 + dy + d) : (z / 26);
    auto res = solve1(blocks, n * 10 + d, nz, i + 1);
    if (res != -1) {
      return res;
    }
  }
  return -1;
}

/* ------------------------------------------------------------------------ */

int
main() {
  auto blocks = Blocks{};
  auto prog = Prog{};
  string line;
  Block b;
  while (std::getline(std::cin, line)) {
    auto ss = std::istringstream{line};
    string inst, op2;
    char op1;
    ss >> inst >> op1 >> op2;

    if (inst == "div" && op1 == 'z') {
      b.div = std::stoi(op2);
    } else if (inst == "add") {
      switch (op1) {
        case 'x': b.dx = std::atoi(op2.c_str()); break;
        case 'y': b.dy = std::atoi(op2.c_str()); break;
        case 'z': blocks.push_back(b); break;
      }
    }

    if (inst == "inp") {
      prog.emplace_back(INP, op1, op1);
    } else if (inst == "add") {
      prog.emplace_back(ADD, op1, op(op2));
    } else if (inst == "mul") {
      prog.emplace_back(MUL, op1, op(op2));
    } else if (inst == "div") {
      prog.emplace_back(DIV, op1, op(op2));
    } else if (inst == "mod") {
      prog.emplace_back(MOD, op1, op(op2));
    } else if (inst == "eql") {
      prog.emplace_back(EQL, op1, op(op2));
    } else {
      println("@@@ FAIL: {}", line);
    }
    println("{} {} {}", name(prog.back().m), prog.back().op1, to_string(prog.back().op2));
  }

  println("@@@ {}", blocks);
  println("1: {}", solve1(blocks));
  println("eval: {}", eval(prog, solve1(blocks)));

  return 0;
}
