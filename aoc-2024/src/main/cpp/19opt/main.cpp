#include <algorithm>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <numeric>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

using Num = std::uint64_t;
using Strings = std::vector<std::string>;

static bool
startsWith(auto b1, auto e1, auto b2, auto e2) {
  while (b1 < e1 && b2 < e2) {
    if (*b1++ != *b2++) {
      return false;
    }
  }
  return b2 == e2 && b1 <= e1;
}

static Num
countMatches(std::string_view text, const Strings &patterns) {
  auto ways = std::vector<Num>(text.size() + 1);
  ways[text.size()] = 1;
  for (int i = text.size() - 1; i >= 0; --i) {
    for (std::string_view p : patterns) {
      if (startsWith(text.begin() + i, text.end(), p.begin(), p.end())) {
        ways[i] += ways[i + p.size()];
      }
    }
  }
  return ways.front();
}

int
main() {
  std::string line;
  std::getline(std::cin, line);
  std::transform(line.begin(), line.end(), line.begin(), [](auto c) { return c == ',' ? ' ' : c; });
  auto ss = std::stringstream(line);
  Strings patterns;
  std::copy(std::istream_iterator<std::string>(ss), std::istream_iterator<std::string>(), std::back_inserter(patterns));
  Strings targets;
  std::copy(
      std::istream_iterator<std::string>(std::cin), std::istream_iterator<std::string>(), std::back_inserter(targets));

  auto counts = targets | std::views::transform([&patterns](std::string_view t) { return countMatches(t, patterns); }) |
      std::ranges::to<std::vector<Num>>();
  auto res1 = std::count_if(counts.begin(), counts.end(), std::identity());
  auto res2 = std::reduce(counts.begin(), counts.end(), Num{});
  std::cout << "1: " << res1 << "\n";
  std::cout << "2: " << res2 << "\n";

  return 0;
}
