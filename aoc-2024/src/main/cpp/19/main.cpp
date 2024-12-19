#include <algorithm>
#include <iostream>
#include <string>
#include <iterator>
#include <sstream>
#include <ranges>
#include <unordered_map>

using Num = uint64_t;
using Strings = std::vector<std::string>;
using Cache = std::unordered_map<std::string_view, Num>;

static Num
countMatches(std::string_view text, const Strings& patterns, Cache& cache) {
  if (text.empty()) {
    return 1;
  }
  auto it = cache.find(text);
  if (it != cache.end()) {
    return it->second;
  }
  Num res = 0;
  for (std::string_view p : patterns) {
    if (text.starts_with(p)) {
      res += countMatches(text.substr(p.size()), patterns, cache);
    }
  }
  cache[text] = res;;
  return res;
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
  std::copy(std::istream_iterator<std::string>(std::cin), std::istream_iterator<std::string>(), std::back_inserter(targets));

  auto counts = targets | std::views::transform([&patterns](std::string_view t) { Cache cache; return countMatches(t, patterns, cache); }) | std::ranges::to<std::vector<Num>>();
  auto res1 = std::count_if(counts.begin(), counts.end(), std::identity());
  auto res2 = std::reduce(counts.begin(), counts.end(), Num{});
  std::cout << "1: " << res1 << "\n";
  std::cout << "2: " << res2 << "\n";

  return 0;
}
