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

auto add = [](Num a, Num b) { return a + b; };
auto mul = [](Num a, Num b) { return a * b; };
const auto OPS = std::vector<std::function<Num(Num, Num)>>{ add, mul };

static bool
inc(auto b, auto e) {
  if (b == e) {
    return false;
  }
  if (++(*b) < OPS.size()) {
    return true;
  }
  *b = 0;
  return inc(++b, e);
}

static bool
check(const Data& d, const std::vector<Num> sel) {
  auto nb = d.nums.begin();
  auto res = *nb++;
  for (auto b = sel.begin(), e = sel.end(); b != e; ++b, ++nb) {
    res = OPS[*b](res, *nb);
  }
  return res == d.res;
}

static bool
check1(const Data& d) {
  std::vector<Num> sel(d.nums.size() - 1);
  do {
    if (check(d, sel)) {
      return true;
    }
  } while (inc(sel.begin(), sel.end()));
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
  for (const auto& d: input) {
    if (check1(d)) {
      res1 += d.res;
    }
  }
  std::cout << "1: " << res1 << "\n";

  Num res2 = 0;
  std::cout << "2: " << res2 << "\n";

  return 0;
}
