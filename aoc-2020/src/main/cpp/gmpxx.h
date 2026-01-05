#include <gmpxx.h>
#include <iostream>

template <>
struct std::hash<mpz_class> {
  std::size_t operator()(const mpz_class &n) const { return n.get_ui(); }
};

inline static auto
operator<=>(const mpz_class &a, const mpz_class &b) {
  return sgn(a - b) <=> 0;
}

template <>
struct std::formatter<mpz_class> {
  template <typename FormatContext>
  constexpr auto parse(FormatContext &ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  constexpr auto format(const mpz_class &n, FormatContext &ctx) const {
    return format_to(ctx.out(), "{}", n.get_str());
  }
};
