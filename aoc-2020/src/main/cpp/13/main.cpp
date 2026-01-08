#include "../utils.h"
#include <regex>

using Num = int64_t;

struct Offset {
  Num m, a;
};

template <>
struct std::formatter<Offset> {
  template <typename FormatContext>
  constexpr auto parse(FormatContext &ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  constexpr auto format(const Offset &o, FormatContext &ctx) const {
    return format_to(ctx.out(), "{{m={}, a={}}}", o.m, o.a);
  }
};

std::tuple<Num, Num, Num>
egcd(Num a, Num b) {
  Num pr = a, r = b, ps = 1, s = 0, pt = 0, t = 1;
  while (r != 0) {
    Num q = pr / r;
    std::tie(pr, r) = std::make_pair(r, pr - q * r);
    std::tie(ps, s) = std::make_pair(s, ps - q * s);
    std::tie(pt, t) = std::make_pair(t, pt - q * t);
  }
  return {pr, ps, pt};
}

static Offset
crt(Offset a, Offset b) {
  auto [g, ma, mb] = egcd(a.m, b.m);
  Num m = a.m * b.m;
  Num r = a.a * b.m * mb + b.a * a.m * ma;
  r = (r % m + m) % m;
  return {m, r};
}

/* ------------------------------------------------------------------------ */

int
main() {
  Num ts;
  Num minId = -1, minDelay = std::numeric_limits<Num>::max();
  string line;
  std::cin >> ts >> line;
  std::vector<Offset> offsets;

  std::regex re("([x0-9-]+)");
  Num d = 0;
  for (auto it = std::sregex_iterator(line.begin(), line.end(), re); it != std::sregex_iterator(); ++it, ++d) {
    if (it->str() == "x") {
      continue;
    }
    Num id = std::stoi(it->str());
    offsets.emplace_back(id, -d);
    Num delay = (((ts + id - 1) / id) * id) - ts;
    if (delay < minDelay) {
      println("@@@ ts={}, id={}, delay={}", ts, id, delay);
      minDelay = delay;
      minId = id;
    }
  }
  println("1: {}", minDelay * minId);
  println("@@@ {}", offsets);

#if 0
  Num x = offsets.front().m;
  Num m = 1;
  for (auto o : offsets) {
    while (x % o.m != ((o.a + o.m) % o.m)) {
      x += m;
    }
    m *= o.m;
    println("@@@ {} -> m={}, x={}", o, m, x);
  }
#endif

  Offset r{1, 0};
  for (auto o : offsets) {
    println("@@@ crt({}, {}) = {}", r, o, crt(r, o));
    r = crt(r, o);
  }
  for (auto o : offsets) {
    println("@@@ {} -> {}", o, (((r.a / o.m) + 1) * o.m) - r.a);
  }
  println("2: {}", r.a);

  return 0;
}
