#include <gmpxx.h>

template <>
struct std::hash<mpz_class> {
  std::size_t operator()(const mpz_class& k) const {
    return k.get_ui();
  }
};

