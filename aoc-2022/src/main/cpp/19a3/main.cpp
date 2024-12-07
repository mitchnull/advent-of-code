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
#include <sstream>
#include <set>
#include <utility>
#include <unordered_map>
#include <regex>
#include <boost/functional/hash.hpp>

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
    default:
      return "?";
  }
}

static constexpr const uint TURNS = 24;

using Counts = std::array<uint8, Type::None>;
using Costs = std::array<Counts, Type::None>;

static constexpr const Counts::value_type Inf = std::numeric_limits<Counts::value_type>::max();

using StateBase = std::pair<Counts, Counts>;
struct State : StateBase {
  State() {}
  State(Counts robots, Counts counts): StateBase(std::move(robots), std::move(counts)) {}

  auto robots() const {
    return first;
  }
  uint robots(Type r) const {
    return first[r];
  }
  auto counts() const {
    return second;
  }
  uint counts(Type t) const {
    return second[t];
  }
};

using Set = std::unordered_set<State, boost::hash<State>>;

static uint
parse(const std::sregex_iterator& it) {
  return std::stoi(it->str());
}

// Blueprint  1: Each ore robot costs 3 ore. Each clay robot costs 4 ore. Each obsidian robot costs 3 ore and 19 clay. Each geode robot costs 3 ore and  8 obsidian.
static Costs
parse(const string& line) {
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
  std::cout
    << "Each ore robot costs " << +costs[Ore][Ore] << " ore. "
    << "Each clay robot costs " << +costs[Clay][Ore] << " ore. "
    << "Each obsidian robot costs " << +costs[Obsidian][Ore] << " ore and " << +costs[Obsidian][Clay] << " clay. "
    << "Each geode robot costs " << +costs[Geode][Ore] << " ore and " << +costs[Geode][Obsidian] << " obsidian.\n";
#endif
  return costs;
}

static bool
canBuild(const Counts& counts, const Costs& costs, Type r) {
  for (Type i: Ingredients) {
    uint c = costs[r][i];
    if (c > 0 && c > counts[i]) {
      return false;
    }
  }
  return true;
}

static Counts
operator+(Counts a, const Counts& b) {
  static const decltype(a[0] + b[0]) Cap = std::numeric_limits<Counts::value_type>::max();
  for (uint i = 0; i < None; ++i) {
    if (a[i] != Inf) {
      a[i] = std::min(Cap, a[i] + b[i]);
    }
  }
  return a;
}

static Counts
operator-(Counts a, const Counts& b) {
  for (uint i = 0; i < None; ++i) {
    if (a[i] != Inf) {
      a[i] -= b[i];
    }
  }
  return a;
}

static Counts
operator+(Counts a, Type r) {
  if (a[r] != Inf) {
    ++a[r];
  }
  return a;
}

static Counts
cap(Counts counts, const Counts& maxNeededRobots, const Counts& robots) {
  for (uint i = 0; i < Geode; ++i) {
    if (robots[i] >= maxNeededRobots[i] && counts[i] >= maxNeededRobots[i]) {
      counts[i] = Inf;
    }
  }
  return counts;
}

static uint
maxGeodes(const Costs& costs, const Counts& maxNeededRobots, const Set& states, uint step = 0) {
  std::cout << "step: " << step << ", states: " << states.size() << "\n";
  if (++step >= TURNS) {
    uint res = 0;
    for (const auto& state: states) {
      res = std::max(res, state.counts(Geode) + state.robots(Geode));
    }
    return res;
  }
  Set nextStates;
  for (const auto& state: states) {
    Counts nextCounts = cap(state.counts() + state.robots(), maxNeededRobots, state.robots());
    nextStates.emplace(state.robots(), nextCounts);
    for (uint i = 0; i < None; ++i) {
      Type r = static_cast<Type>(i);
      if (state.robots(r) >= maxNeededRobots[r]) {
        continue;
      }
      if (canBuild(state.counts(), costs, r)) {
        nextStates.emplace(state.robots() + r, nextCounts - costs[r]);
      }
    }
  }
  return maxGeodes(costs, maxNeededRobots, nextStates, step);
}

static Counts
calcMaxNeededRobots(const Costs& costs) {
  Counts res{};
  for (Counts cost: costs) {
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
  while (std::getline(std::cin, line)) {
    blueprints.push_back(parse(line));
  }
  Set initStates{State{Counts{} + Ore, {}}};
  uint res = 0;
  for (uint i = 0; i < blueprints.size(); ++i) {
    const Costs& bp = blueprints[i];
    Counts maxNeededRobots = calcMaxNeededRobots(bp);
    uint bpm = maxGeodes(bp, maxNeededRobots, initStates);
    std::cout << (i + 1) << ": " << bpm << "\n";
    res += (i + 1) * bpm;
  }
  std::cout << res << "\n";
  return 0;
}
