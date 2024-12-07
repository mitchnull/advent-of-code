#include <iostream>
#include <cstdint>
#include <string>
#include <sstream>
#include <functional>

using Num = std::uint64_t;

struct Data {
  Num res;
  std::vector<Num> nums;
};

using Ops = std::vector<std::function<Num(Num, Num)>>;

auto add = [](Num a, Num b) { return a + b; };
auto mul = [](Num a, Num b) { return a * b; };
auto concat = [](Num a, Num b) { return std::stol(std::to_string(a) + std::to_string(b)); };

const auto OPS1 = Ops{ add, mul };
const auto OPS2 = Ops{ add, mul, concat };

static bool
inc(auto b, auto e, std::size_t numOps) {
  if (b == e) {
    return false;
  }
  if (++(*b) < numOps) {
    return true;
  }
  *b = 0;
  return inc(++b, e, numOps);
}

static bool
check(const Data& d, const Ops& ops, const std::vector<Num> sel) {
  auto nb = d.nums.begin();
  auto res = *nb++;
  for (auto b = sel.begin(), e = sel.end(); b != e; ++b, ++nb) {
    res = ops[*b](res, *nb);
  }
  return res == d.res;
}

static bool
check(const Data& d, const Ops& ops) {
  std::vector<Num> sel(d.nums.size() - 1);
  do {
    if (check(d, ops, sel)) {
      return true;
    }
  } while (inc(sel.begin(), sel.end(), ops.size()));
  return false;
}


// ------------------------------------------------------------------------ //

int
main() {

  std::vector<Data> input;
  std::string line;
  while (std::getline(std::cin, line)) {
    std::stringstream ss{line};
    Num num;
    Data d;
    ss >> d.res;
    ss.ignore(1, ' ');
    while (ss >> num) {
      d.nums.push_back(num);
    }
    input.emplace_back(d);
  }

  Num res1 = 0;
  Num res2 = 0;
  for (const auto& d: input) {
    if (check(d, OPS1)) {
      res1 += d.res;
    }
    if (check(d, OPS2)) {
      res2 += d.res;
    }
  }
  std::cout << "1: " << res1 << "\n";
  std::cout << "2: " << res2 << "\n";

  return 0;
}
