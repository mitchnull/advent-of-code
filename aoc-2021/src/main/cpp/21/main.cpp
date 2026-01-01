#include "../utils.h"
#include <unordered_map>

using Num = int64_t;

static Num
solve1(Num p1, Num p2) {
  Num count = 0, die = 0, s1 = 0, s2 = 0;
  while (true) {
    for (int i = 0; i < 3; ++i) {
      die = (die + 1) % 100;
      p1 = (p1 + die - 1) % 10 + 1;
      ++count;
    }
    s1 += p1;
    if (s1 >= 1000) {
      return s2 * count;
    }
    std::swap(p1, p2);
    std::swap(s1, s2);
  }
}

/* ------------------------------------------------------------------------ */

struct Player {
  int8_t pos;
  int8_t score = 0;
  friend auto operator<=>(const Player &, const Player &) = default;
};
using State = std::array<Player, 2>;

template <>
struct std::hash<State> {
  std::size_t operator()(const State &s) const {
    return (s[0].pos << 24) | (s[0].score << 16) | (s[1].pos << 8) | s[1].score;
  }
};

static Num
solve2(int8_t p1, int8_t p2, int8_t goal) {
  std::unordered_map<int, int> rolls;
  for (int d1 = 1; d1 <= 3; ++d1) {
    for (int d2 = 1; d2 <= 3; ++d2) {
      for (int d3 = 1; d3 <= 3; ++d3) {
        ++rolls[d1 + d2 + d3];
      }
    }
  }
  std::array<Num, 2> res = {};
  std::unordered_map<State, Num> states;
  states[State{Player{p1, 0}, Player{p2, 0}}] = 1;
  for (bool t = 0; !states.empty(); t = !t) {
    std::unordered_map<State, Num> next;
    for (const auto &[s, sc] : states) {
      for (const auto &[r, rc] : rolls) {
        int8_t pos = (s[t].pos + r - 1) % 10 + 1;
        int8_t score = s[t].score + pos;
        if (score >= goal) {
          res[t] += sc * rc;
        } else {
          State ns = (t == 0) ? State{Player{pos, score}, s[1]} : State{s[0], Player{pos, score}};
          next[ns] += sc * rc;
        }
      }
    }
    states = std::move(next);
  }
  return res[0];
}

/* ------------------------------------------------------------------------ */

int
main() {
  //  Player 1 starting position: 4
  string line;
  Num p1, p2;
  std::getline(std::cin, line);
  p1 = std::stoi(line.substr(string{"Player 1 starting position: "}.size()));
  std::getline(std::cin, line);
  p2 = std::stoi(line.substr(string{"Player 2 starting position: "}.size()));

  println("1: {}", solve1(p1, p2));
  println("2: {}", solve2(p1, p2, 21));

  return 0;
}
