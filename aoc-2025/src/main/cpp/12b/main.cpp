#include "../utils.h"
#include <iterator>
#include <numeric>
#include <sstream>

using Num = int64_t;
using Box = Grid<>;
using Boxes = std::vector<Box>;
using Region = Grid<>;
struct Task {
  int w, h;
  std::vector<int> items;
};
using Tasks = std::vector<Task>;
using BoxConfigs = std::vector<Boxes>;

struct State {
  int item, x = 0, y = 0, conf = 0;
};
using States = std::vector<State>;

template <>
struct std::formatter<State> {
  constexpr auto parse(std::format_parse_context &ctx) { return ctx.begin(); }

  auto format(const State &s, std::format_context &ctx) const {
    return std::format_to(ctx.out(), "{{{}, {}, {}, {}}}", s.item, s.x, s.y, s.conf);
  }
};

template <>
struct std::formatter<States> {
  constexpr auto parse(std::format_parse_context &ctx) { return ctx.begin(); }

  auto format(const States &p, std::format_context &ctx) const {
    std::format_to(ctx.out(), "[");
    std::string sep = "";
    for (auto s : p) {
      std::format_to(ctx.out(), "{}{}", sep, s);
      sep = ", ";
    }
    return std::format_to(ctx.out(), "]");
  }
};

static Box
rot(const Box &b) {
  Box r(b.h(), b.w(), b[-1, -1], b[-1, -1]);
  for (int y = 0; y < b.h(); ++y) {
    for (int x = 0; x < b.w(); ++x) {
      r[y, r.h() - x - 1] = b[x, y];
    }
  }
  return r;
}

static Box
flip(const Box &b) {
  Box r(b.w(), b.h(), b[-1, -1], b[-1, -1]);
  for (int y = 0; y < b.h(); ++y) {
    for (int x = 0; x < b.w(); ++x) {
      r[r.w() - x - 1, y] = b[x, y];
    }
  }
  return r;
}

static Boxes
rotFlip(Box b) {
  Boxes res{b};
  for (int i = 0; i < 3; ++i) {
    b = rot(b);
    if (std::find(res.begin(), res.end(), b) == res.end()) {
      res.push_back(b);
    }
  }
  b = flip(rot(b));
  if (std::find(res.begin(), res.end(), b) == res.end()) {
    res.push_back(b);
  }
  for (int i = 0; i < 3; ++i) {
    b = rot(b);
    if (std::find(res.begin(), res.end(), b) == res.end()) {
      res.push_back(b);
    }
  }
  return res;
}

static bool
check(const Task &t, const BoxConfigs &boxConfigs, const States &p) {
  Region r(t.w, t.h, '.');
  for (const auto &s : p) {
    const auto &b = boxConfigs[s.item][s.conf];
    for (int y = 0; y < b.h(); ++y) {
      for (int x = 0; x < b.w(); ++x) {
        if (b[x, y] == '#' && r[s.x + x, s.y + y] == '#') {
          return false;
        }
        r[s.x + x, s.y + y] = b[x, y];
      }
    }
  }
  return true;
}

static bool
inc(const Task &t, const BoxConfigs &boxConfigs, States &p) {
  for (auto &s : p) {
    const auto &b = boxConfigs[s.item][s.conf];
    if (++s.x <= t.w - b.w()) {
      return true;
    }
    s.x = 0;
    if (++s.y <= t.h - b.h()) {
      return true;
    }
    s.y = 0;
    if (++s.conf < boxConfigs[s.item].size()) {
      return true;
    }
    s.conf = 0;
  }
  return false;
}

static bool
solve1(const Task &t, const BoxConfigs &boxConfigs, const std::vector<int> &boxSizes) {
  const int N = t.items.size();
  int bs = 0;
  for (int i = 0; i < N; ++i) {
    bs += t.items[i] * boxSizes[i];
  }
  if (bs > (t.w * t.h)) {
    return false;
  }
  std::vector<State> p;
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < t.items[i]; ++j) {
      p.push_back(State{i});
    }
  }
  println("@@@ solve: {}x{}: {}", t.w, t.h, t.items);
  do {
    print("@@@ p: {}\r", p | views::transform([](const auto &s) { return s.conf; }));
    // println("@@@ p: {}", p);
    if (check(t, boxConfigs, p)) {
      println("\n@@@ fits");
      return true;
    }
  } while (inc(t, boxConfigs, p));
  println("\n@@@ nope");
  return false;
}

/* ------------------------------------------------------------------------ */

int
main() {
  std::string line;
  Boxes boxes;
  Tasks tasks;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      continue;
    }
    std::istringstream ss{line};
    int w, h;
    char ch;
    ss >> w >> ch;
    if (ch == ':') {
      std::vector<std::string> lines;
      while (std::getline(std::cin, line) && !line.empty()) {
        lines.push_back(line);
      }
      boxes.push_back(Box(lines, '.'));
    } else {
      ss >> h >> ch;
      std::vector<int> items{std::istream_iterator<int>{ss}, std::istream_iterator<int>()};
      tasks.emplace_back(w, h, items);
    }
  }
  BoxConfigs boxConfigs = boxes | views::transform(rotFlip) | ranges::to<BoxConfigs>();
  auto boxSizes = boxes | views::transform([](auto b) { return std::count(b.begin(), b.end(), '#'); }) |
      ranges::to<std::vector<int>>();

  // auto res1 = std::transform_reduce(tasks.begin(), tasks.end(), 0, std::plus<>{}, [&](const auto& t) { return
  // solve1(t, boxConfigs, boxSizes); });

  println("1: {}", 0);
  println("2: {}", 0);

  int max = 0, mw = 0, mh = 0;
  for (auto t : tasks) {
    max = std::max(max, t.w * t.h);
    mw = std::max(mw, t.w);
    mh = std::max(mh, t.h);
  }
  println("@@@ max: {}, mw: {}, mh: {}", max, mw, mh);
  return 0;
}
