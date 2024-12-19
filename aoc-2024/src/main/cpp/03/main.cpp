#include <cstdint>
#include <iostream>
#include <iterator>
#include <regex>
#include <string>

using uint = std::uint32_t;


int
main() {
  auto text = std::string(std::istreambuf_iterator<char>(std::cin), {});

  auto r = std::regex("mul[(]([0-9]{1,3}),([0-9]{1,3})[)]");

  int res1 = 0;
  for (auto it = std::sregex_token_iterator(text.begin(), text.end(), r, {1, 2}), end = std::sregex_token_iterator(); it != end; ++it) {
    res1 += std::stoi(*it) * std::stoi(*++it);
  }
  std::cout << "1: " << res1 << "\n";

  // ------------------------------------------------------------------------ //

  auto dr = std::regex("don't[(][)][^]*?(do[(][)]|$)");
  std::string text2 = std::regex_replace(text, dr, "");

  int res2 = 0;
  for (auto it = std::sregex_token_iterator(text2.begin(), text2.end(), r, {1, 2}), end = std::sregex_token_iterator(); it != end; ++it) {
    res2 += std::stoi(*it) * std::stoi(*++it);
  }
  std::cout << "2: " << res2 << "\n";

  return 0;
}
