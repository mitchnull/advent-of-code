#include <ios>
#include <iostream>
#include <cctype>
#include <limits>
#include <deque>
#include <string>
#include <vector>
#include <bitset>
#include <algorithm>
#include <ranges>
#include <charconv>
#include <numeric>
#include <memory>
#include <unordered_set>
#include <string_view>
#include <variant>

using uint8 = std::uint8_t;
using int8 = std::int8_t;
using uint32 = std::uint32_t;
using int32 = std::int32_t;
using uint64 = std::uint64_t;
using int64 = std::int64_t;

using uint = uint32;
using string = std::string;
using namespace std::literals;
using size_t = std::size_t;

struct Item;
using List = std::vector<Item>;


struct Item {
  std::variant<std::monostate, int, List> value;

  int
  n() const {
    return std::get<int>(value);
  }

  const List&
  list() const {
    return std::get<List>(value);
  }

  bool
  isList() const {
    return std::holds_alternative<List>(value);
  }

  void
  add(Item&& i) {
    if (i.value.index() != 0) {
      std::get<List>(value).push_back(std::forward<Item>(i));
    }
  }
};

static void
print(const Item& i) {
  if (i.isList()) {
    std::cout << "[";
    bool first = true;
    for (const auto& v: i.list()) {
      if (first) {
        first = false;
      } else {
        std::cout << ',';
      }
      print(v);
    }
    std::cout << "]";
  } else {
    std::cout << i.n();
  }
}

static void
println(const Item& i) {
  print(i);
  std::cout << "\n";
}

static int
cmp(const List& a, const List& b);

static int
cmp(const Item& a, const Item& b) {
  if (a.isList()) {
    if (b.isList()) {
      return cmp(a.list(), b.list());
    } else {
      return cmp(a.list(), List{{b.n()}});
    }
  } else if (b.isList()) {
      return cmp(List{{a.n()}}, b.list());
  } else {
    return a.n() - b.n();
  }
}

static int
cmp(const List& a, const List& b) {
  for (uint i = 0; i < a.size() && i < b.size(); ++i) {
    int r = cmp(a[i], b[i]);
    if (r != 0) {
      return r;
    }
  }
  return (a.size() - b.size());
}

static std::pair<Item, uint> 
parse(const string& line, uint p = 0) {
  Item res;
  if (line[p] == '[') {
    // std::cerr << "found list: " << line.substr(p) << "\n";
    res = {List{}};
    while (line[p] != ']') {
      auto r = parse(line, p + 1);
      res.add(std::move(r.first));
      p = r.second;
    }
    ++p;
  } else {
    uint e = line.find_first_of("],", p);
    if (p < e) {
      // std::cerr << "found number: " << line.substr(p, e - p) << "\n";
      res = {std::stoi(line.substr(p, e - p))};
    }
    p = e;
  }
  return {res, p};
}

int
main() {
  Item d1 = parse("[[2]]", 0).first;
  Item d2 = parse("[[6]]", 0).first;
  std::vector<Item> list{d1, d2};
  string line;
  while (std::cin >> std::ws >> line) {
    Item i = parse(line).first;
    list.push_back(std::move(i));
  }
  std::sort(list.begin(), list.end(), [](const Item& a, const Item& b) { return cmp(a, b) < 0; });
  int d1i = 0, d2i = 0;
  int j = 0;
  for (const auto& i: list) {
    ++j;
    if (cmp(i, d1) == 0) {
      d1i = j;
    } else if (cmp(i, d2) == 0) {
      d2i = j;
    }
  }
  std::cout << d1i * d2i << "\n";
}
