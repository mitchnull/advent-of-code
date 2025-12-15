#include <iostream>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <queue>
#include <sstream>
#include <numeric>

/* ------------------------------------------------------------------------ */

using Num = std::int64_t;
using std::string;
using Inputs = std::unordered_map<string, std::pair<bool, Num>>;
using Outputs = std::vector<string>;
using Counts = std::array<Num, 2>;

struct Pulse {
  string src;
  string dst;
  bool level;
};

using Queue = std::queue<Pulse>;

enum ModuleType {
  Broadcaster,
  FlipFlop,
  Conjunction,
};

struct Module {
  string name;
  ModuleType type;
  Outputs outputs;
  Inputs inputs;
  bool level;
  Num period;

  void process(Pulse p, Queue& queue, Num pc) {
    bool outLevel = p.level;
    switch (type) {
      case Broadcaster:
        break;
      case FlipFlop:
        if (p.level) {
          return;
        }
        level = !level;
        outLevel = level;
        break;
      case Conjunction:
        inputs[p.src].first = p.level;
        if (p.level && !inputs[p.src].second) {
          inputs[p.src].second = pc;
          period = std::reduce(inputs.begin(), inputs.end(), Num{1}, [](auto a, auto b) { return a * b.second.second; });
        }
        outLevel = std::find_if(inputs.begin(), inputs.end(), [](auto& it) { return !it.second.first; }) != inputs.end();
        break;
    }
    for (auto& out : outputs) {
      queue.emplace(name, out, outLevel);
    }
  }
};

using Modules = std::unordered_map<string, Module>;

static Counts
pulse(Modules& modules, Num pc) {
  std::array<Num, 2> counts = {};
  Queue queue;
  queue.emplace("button" ,"broadcaster", false);
  while (!queue.empty()) {
    Pulse p = queue.front();
    queue.pop();
    // std::cout << p.src << (p.level ? " -high-> " : " -low-> ") << p.dst << std::endl;
    ++counts[p.level];
    auto& m = modules[p.dst];
    m.process(p, queue, pc);
  }
  return counts;
}

static Num
solve1(Modules modules) {
  Counts allCounts = {};
  for (int i = 1; i <= 1000; ++i) {
    auto counts = pulse(modules, i);
    allCounts[0] += counts[0];
    allCounts[1] += counts[1];
  }
  return allCounts[0] * allCounts[1];
}

static Num
solve2(Modules modules) {
  Module& rx = modules["rx"];
  Module& rxi = modules[rx.inputs.begin()->first];
  for (int i = 1; rxi.period == 0; ++i) {
    pulse(modules, i);
  }
  return rxi.period;
}

/* ------------------------------------------------------------------------ */

int
main() {
  string line;
  Modules modules;
  while (std::getline(std::cin, line)) {
    std::transform(line.begin(), line.end(), line.begin(), [](auto c) { return (c == ',') ? ' ' : c; });
    auto ss = std::stringstream(line);
    std::string name;
    ModuleType type = Broadcaster;
    ss >> name;
    switch (name.front()) {
      case '%':
        name = name.substr(1);
        type = FlipFlop;
        break;
      case '&':
        name = name.substr(1);
        type = Conjunction;
        break;
    }
    Module& m = modules[name];
    m.name = name;
    m.type = type;
    string out;
    ss >> out;
    while (ss >> out) {
      m.outputs.push_back(out);
      Module& t = modules[out];
      t.inputs[name] = {false, 0};
    }
  }

  Num res1 = solve1(modules);
  std::cout << res1 << "\n";

  Num res2 = solve2(modules);
  std::cout << res2 << "\n";
}
