#include "../utils.h"
#include <regex>

using Num = int64_t;

/* ------------------------------------------------------------------------ */

int
main() {
  Num ts;
  Num minId = -1, minDelay = std::numeric_limits<Num>::max();
  string line;
  std::cin >> ts >> line;

  std::regex re("([0-9-]+)");
  for (auto it = std::sregex_iterator(line.begin(), line.end(), re); it != std::sregex_iterator(); ++it) {
    Num id = std::stoi(it->str());
    Num delay = (((ts + id - 1) / id) * id) - ts;
    if (delay < minDelay) {
      println("@@@ ts={}, id={}, delay={}", ts, id, delay);
      minDelay = delay;
      minId = id;
    }
  }
  println("1: {}", minDelay * minId);

  return 0;
}
