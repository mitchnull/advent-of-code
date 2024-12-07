#include <iostream>
#include <cstdint>
#include <string>
#include <sstream>

using Num = std::uint64_t;

struct Data {
  Num res;
  std::vector<Num> nums;
};

static bool
check1(Num res, auto b, auto e) {
  Num n = *b++;
  if (b == e) {
    return res == n;
  }
  return
    (res >= n && check1(res - n, b, e)) ||
    (res % n == 0 && check1(res / n, b, e));
}

static bool
check2(Num res, auto b, auto e) {
  Num n = *b++;
  if (b == e) {
    return res == n;
  }
  Num mask = std::pow(10, std::floor(std::log10(n)) + 1);
  return
    (res >= n && check2(res - n, b, e)) ||
    (res % n == 0 && check2(res / n, b, e)) ||
    (res % mask == n && check2(res / mask, b, e));

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
    res1 += d.res * check1(d.res, d.nums.rbegin(), d.nums.rend());
    res2 += d.res * check2(d.res, d.nums.rbegin(), d.nums.rend());
  }
  std::cout << "1: " << res1 << "\n";
  std::cout << "2: " << res2 << "\n";

  return 0;
}
