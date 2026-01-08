#include "../utils.h"
#include "../gmpxx.h"
#include <regex>

using Num = mpz_class;

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
    std::tie(pr, r) = std::make_pair(r, Num{pr - q * r});
    std::tie(ps, s) = std::make_pair(s, Num{ps - q * s});
    std::tie(pt, t) = std::make_pair(t, Num{pt - q * t});
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
  Num minId = -1, minDelay = std::numeric_limits<int>::max();
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
      minDelay = delay;
      minId = id;
    }
  }
  println("1: {}", Num{minDelay * minId});

  Offset r{1, 0};
  for (auto o : offsets) {
    r = crt(r, o);
  }
  println("2: {}", r.a);

  return 0;
}
