#include <ios>
#include <iostream>
#include <cctype>
#include <cstdint>
#include <limits>
#include <deque>
#include <string>
#include <vector>
#include <map>
#include <bitset>
#include <algorithm>
#include <ranges>
#include <unordered_map>
#include <sstream>
#include <charconv>

using uint8 = std::uint8_t;
using int8 = std::int8_t;
using uint32 = std::uint32_t;
using int32 = std::int32_t;
using uint64 = std::uint64_t;
using int64 = std::int64_t;

using uint = uint32;

using Num = int64;

namespace ranges = std::ranges;
namespace views = ranges::views;

enum HandType {
  HighCard,
  OnePair,
  TwoPairs,
  ThreeOfAKind,
  FullHouse,
  FourOfAKind,
  FiveOfAKind,
};

static constexpr char J = 1;

static char
map(char c) {
  switch (c) {
    case 'T': return 10;
    case 'J': return J;
    case 'Q': return 12;
    case 'K': return 13;
    case 'A': return 14;
    default: return c - '0';
  }
}

struct Hand {
  HandType type;
  std::string hand;
  Num bid;

  friend auto operator<=>(const Hand&, const Hand&) = default;
};

static HandType
type(const std::string& hand) {
  std::map<char, int> counts;
  int js = 0;
  for (char c : hand) {
    ++counts[c];
    if (c == J) {
      ++js;
    }
  }
  switch (counts.size()) {
    case 1:
      return FiveOfAKind;
    case 2: {
      int count = *ranges::max_element(views::values(counts));
      if (js) {
        return FiveOfAKind;
      }
      if (count == 4) {
        return FourOfAKind;
      } else {
        return FullHouse;
      }
    }
    case 3: {
      int count = *ranges::max_element(views::values(counts));
      if (count == 3) {
        return js ? FourOfAKind : ThreeOfAKind;
      } else if (js == 2) {
        return FourOfAKind;
      } else if (js == 1) {
        return FullHouse;
      } else {
        return TwoPairs;
      }
    }
    case 4:
      return js ? ThreeOfAKind : OnePair;
    default:
      return js ? OnePair : HighCard;
  }
}

int
main() {
  std::string hand;
  Num bid;

  std::vector<Hand> hands;
  while (std::cin >> hand >> bid) {
    std::transform(hand.begin(), hand.end(), hand.begin(), map);
    hands.emplace_back(type(hand), hand, bid);
  }
  std::sort(hands.begin(), hands.end());
  Num res = 0;
  int rank = 0;
  for (const auto& hand: hands) {
    res += ++rank * hand.bid;
  }
  std::cout << res << "\n";
  return 0;
}
