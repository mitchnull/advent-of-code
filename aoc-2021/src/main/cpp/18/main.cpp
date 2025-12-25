#include "../utils.h"
#include <iterator>

using Num = int64_t;
using SFN = string;

static int
findSecond(const SFN &n, int from) {
  int depth = 0;
  for (int i = from; i < n.size(); ++i) {
    switch (n[i]) {
      case '[': ++depth; break;
      case ']': --depth; break;
      case ',': {
        if (depth == 0) {
          return i + 1;
        }
      }
    }
  }
  return -1;
}

static std::pair<int, int>
findPrevNum(const SFN &n, int p) {
  auto l = n.find_last_of("0123456789", p);
  if (l == SFN::npos) {
    return {-1, -1};
  }
  return {n.find_last_not_of("0123456789", l) + 1, l + 1};
}

static std::pair<int, int>
findNextNum(const SFN &n, int p) {
  auto f = n.find_first_of("0123456789", p);
  if (f == string::npos) {
    return {-1, -1};
  }
  return {f, n.find_first_not_of("0123456789", f)};
}

static std::pair<SFN, bool>
explode(SFN n) {
  int depth = 0;
  for (int i = 0; i < n.size(); ++i) {
    if (n[i] == '[' && ++depth > 4) {
      int a = std::stoi(n.substr(i + 1));
      int j = findSecond(n, i + 1);
      size_t jlen;
      int b = std::stoi(n.substr(j), &jlen);
      n = n.substr(0, i) + "0" + n.substr(j + jlen + 1);
      auto [pb, pe] = findPrevNum(n, i - 1);
      auto [nb, ne] = findNextNum(n, i + 1);
      if (nb != -1) {
        auto nn = std::stoi(n.substr(nb)) + b;
        n = n.substr(0, nb) + std::to_string(nn) + n.substr(ne);
      }
      if (pb != -1) {
        auto pn = std::stoi(n.substr(pb)) + a;
        n = n.substr(0, pb) + std::to_string(pn) + n.substr(pe);
      }
      return {n, true};
    } else if (n[i] == ']') {
      --depth;
    }
  }
  return {n, false};
}

static std::pair<SFN, bool>
split(SFN n) {
  for (auto [nb, ne] = findNextNum(n, 0); nb != -1; std::tie(nb, ne) = findNextNum(n, ne)) {
    auto nn = std::stoi(n.substr(nb));
    if (nn > 9) {
      return {std::format("{}[{},{}]{}", n.substr(0, nb), nn / 2, (nn + 1) / 2, n.substr(ne)), true};
    }
  }
  return {n, false};
}

static SFN
reduce(SFN n) {
  bool changed = false;
  do {
    std::tie(n, changed) = explode(n);
    if (changed) {
      continue;
    }
    std::tie(n, changed) = split(n);
  } while (changed);
  return n;
}

static SFN
operator+(const SFN &a, const SFN &b) {
  return reduce(std::format("[{},{}]", a, b));
}

static Num
magnitude(const SFN &n, int i = 0) {
  if (std::isdigit(n[i])) {
    return std::stoi(n.substr(i));
  }
  int j = findSecond(n, ++i);
  return 3 * magnitude(n, i) + 2 * magnitude(n, j);
}

static Num
solve1(const std::vector<SFN> &input) {
  SFN sum = input.front();
  for (int i = 1; i < input.size(); ++i) {
    sum = sum + input[i];
  }
  return magnitude(sum);
}

static Num
solve2(const std::vector<SFN> &input) {
  Num res = 0;
  for (int i = 0; i < input.size(); ++i) {
    for (int j = i + 1; j < input.size(); ++j) {
      res = std::max(res, magnitude(input[i] + input[j]));
      res = std::max(res, magnitude(input[j] + input[i]));
    }
  }
  return res;
}

/* ------------------------------------------------------------------------ */

int
main() {
  std::vector<SFN> input;
  std::copy(std::istream_iterator<string>(std::cin), std::istream_iterator<string>(), std::back_inserter(input));

  println("1: {}", solve1(input));
  println("2: {}", solve2(input));

  return 0;
}
