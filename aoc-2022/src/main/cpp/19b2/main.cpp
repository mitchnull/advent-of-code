#include <algorithm>
#include <bitset>
#include <cctype>
#include <charconv>
#include <deque>
#include <ios>
#include <iostream>
#include <limits>
#include <memory>
#include <numeric>
#include <ranges>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

using uint8 = std::uint8_t;
using int8 = std::int8_t;
using uint16 = std::uint16_t;
using int16 = std::int16_t;
using uint32 = std::uint32_t;
using int32 = std::int32_t;
using uint64 = std::uint64_t;
using int64 = std::int64_t;

using uint = uint32;
using string = std::string;
using namespace std::literals;
using size_t = std::size_t;

using Type = enum {
  Ore,
  Clay,
  Obsidian,
  Geode,
  None,
};

static constexpr const std::array<Type, None - 1> Ingredients{Ore, Clay, Obsidian};

static const char *
toString(Type t) {
  switch (t) {
    case Ore: return "Ore";
    case Clay: return "Clay";
    case Obsidian: return "Obsidian";
    case Geode: return "Geode";
    case None: return "None";
    default: return "?";
  }
}

static constexpr const uint TURNS = 32;

using Count = uint;
using Counts = std::array<Count, Type::None>;
using Costs = std::array<Counts, Type::None>;

struct State {
  uint step;
  Counts robots, counts;
};

using Set = std::vector<State>;

static uint
parse(const std::sregex_iterator &it) {
  return std::stoi(it->str());
}

// Blueprint  1: Each ore robot costs 3 ore. Each clay robot costs 4 ore. Each obsidian robot costs 3 ore and 19 clay.
// Each geode robot costs 3 ore and  8 obsidian.
static Costs
parse(const string &line) {
  Costs costs{};
  std::regex numMatch("[0-9]+");
  auto it = std::sregex_iterator(line.begin(), line.end(), numMatch);
  costs[Ore][Ore] = parse(++it);
  costs[Clay][Ore] = parse(++it);
  costs[Obsidian][Ore] = parse(++it);
  costs[Obsidian][Clay] = parse(++it);
  costs[Geode][Ore] = parse(++it);
  costs[Geode][Obsidian] = parse(++it);
#if 1
  std::cout << "Each ore robot costs " << +costs[Ore][Ore] << " ore. "
            << "Each clay robot costs " << +costs[Clay][Ore] << " ore. "
            << "Each obsidian robot costs " << +costs[Obsidian][Ore] << " ore and " << +costs[Obsidian][Clay]
            << " clay. "
            << "Each geode robot costs " << +costs[Geode][Ore] << " ore and " << +costs[Geode][Obsidian]
            << " obsidian.\n";
#endif
  return costs;
}

static uint
canBuild(const Costs &costs, const Counts &robots, const Counts &counts, Type r) {
  uint res = 0;
  for (Type i : Ingredients) {
    uint c = costs[r][i];
    uint needed = (c > counts[i]) ? c - counts[i] : 0;
    if (needed == 0) {
      continue;
    }
    uint nr = robots[i];
    if (nr == 0) {
      return TURNS; // no way to build it
    }
    res = std::max(res, (needed + nr - 1) / nr);
  }
  return res + 1;
}

static Counts
operator+(Counts a, const Counts &b) {
  for (uint i = 0; i < None; ++i) {
    a[i] += b[i];
  }
  return a;
}

static Counts
operator-(Counts a, const Counts &b) {
  for (uint i = 0; i < None; ++i) {
    a[i] -= b[i];
  }
  return a;
}

static Counts
operator+(Counts a, Type r) {
  ++a[r];
  return a;
}

static Counts
operator*(Counts a, uint n) {
  for (uint i = 0; i < None; ++i) {
    a[i] *= n;
  }
  return a;
}

static Count
maxGeodes(const Costs &costs, const Counts &maxNeededRobots, const State &initState) {
  Set states{initState};
  Count res = 0;
  while (!states.empty()) {
    State state = states.back();
    states.pop_back();
    uint size = states.size();
    for (uint i = 0; i < None; ++i) {
      Type r = static_cast<Type>(i);
      if (state.robots[r] >= maxNeededRobots[r]) {
        // std::cout << "step " << state.step << ": already have " << maxNeededRobots[r] << " " << toString(r) << "
        // robots\n";
        continue;
      }
      uint steps = canBuild(costs, state.robots, state.counts, r);
      if (state.step + steps <= TURNS) {
        states.push_back(State{state.step + steps, state.robots + r, state.counts + state.robots * steps - costs[r]});
      }
    }
    if (states.size() == size) {
      res = std::max(res, (state.counts + state.robots * (TURNS - state.step + 1))[Geode]);
    }
  }
  return res;
}

static Counts
calcMaxNeededRobots(const Costs &costs) {
  Counts res{};
  for (Counts cost : costs) {
    for (uint i = 0; i < Geode; ++i) {
      Type r = static_cast<Type>(i);
      res[r] = std::max(res[r], cost[r]);
    }
  }
  res[Geode] = TURNS;
  return res;
}

int
main() {
  std::vector<Costs> blueprints;
  string line;
  int intact = 3;
  while (std::getline(std::cin, line)) {
    blueprints.push_back(parse(line));
    if (--intact == 0) {
      break;
    }
  }
  State initState{1, Counts{} + Ore, {}};
  uint res = 1;
  for (uint i = 0; i < blueprints.size(); ++i) {
    const Costs &bp = blueprints[i];
    Counts maxNeededRobots = calcMaxNeededRobots(bp);
    uint bpm = maxGeodes(bp, maxNeededRobots, initState);
    std::cout << (i + 1) << ": " << bpm << "\n";
    res *= bpm;
  }
  std::cout << res << "\n";
  return 0;
}
