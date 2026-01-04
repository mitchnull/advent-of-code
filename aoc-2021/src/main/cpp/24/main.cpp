#include "../utils.h"
#include <sstream>
#include <variant>

using Num = int64_t;

enum Mnemoic {
  INP,
  ADD,
  MUL,
  DIV,
  MOD,
  EQL,
};

using Op = std::variant<char, Num>;

struct Inst {
  Mnemoic m;
  char op1;
  Op op2;
};

using Prog = std::vector<Inst>;
using Regs = std::vector<Num>;

static Op
op(string v) {
  return std::isalpha(v[0]) ? v[0] : std::stol(v);
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
      case INP:
        r[i.op1] = *it++; break;
      case ADD:
        r[i.op1] += v(r, i.op2); break;
      case MUL:
        r[i.op1] *= v(r, i.op2); break;
      case DIV:
        r[i.op1] /= v(r, i.op2); break;
      case MOD:
        r[i.op1] %= v(r, i.op2); break;
      case EQL:
        r[i.op1] = r[i.op1] == v(r, i.op2); break;
    }
  }
  return !r['z'];
}

static Num
solve1(const Prog &prog) {
  Num watch = 0;
  for (Num n = 99999999999999L; n >= 11111111111111; --n) {
    if (++watch % 1000000 == 0) {
      println("@@@ {}", n);
    }
    if (eval(prog, n)) {
      return n;
    }
  }
  return -1;
}

/* ------------------------------------------------------------------------ */

int
main() {
  auto prog = Prog{};
  string line;
  while (std::getline(std::cin, line)) {
    auto ss = std::istringstream{line};
    string inst, op2;
    char op1;
    ss >> inst >> op1 >> op2;
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
      prog.emplace_back(MOD, op1, op(op2));
    }
  }

  println("1: {}", solve1(prog));

  return 0;
}
