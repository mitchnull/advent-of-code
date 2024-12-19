#include <gmpxx.h>
#include <iostream>

static inline std::ostream&
operator<<(std::ostream& os, const mpz_class& n) {
  return os << n.get_str();
}

static inline std::istream&
operator>>(std::istream& is, mpz_class& n) {
  std::string str;
  if (is >> str) {
    n.set_str(str, 10);
  }
  return is;
}

template <>
struct std::hash<mpz_class> {
  std::size_t operator()(const mpz_class& n) const {
    return n.get_ui();
  }
};

