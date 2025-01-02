#include <iostream>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <queue>
#include <sstream>

/* ------------------------------------------------------------------------ */

using std::string;
using Inputs = std::unordered_map<string, bool>;
using Outputs = std::vector<string>;
using Num = std::int64_t;
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
  bool receivedLow;

  void process(Pulse p, Queue& queue) {
    receivedLow |= !p.level;
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
        inputs[p.src] = p.level;
        outLevel = std::find_if(inputs.begin(), inputs.end(), [](auto& it) { return !it.second; }) != inputs.end();
        break;
    }
    for (auto& out : outputs) {
      queue.emplace(name, out, outLevel);
    }
  }
};

using Modules = std::unordered_map<string, Module>;

static Counts
pulse(Modules& modules) {
  std::array<Num, 2> counts = {};
  Queue queue;
  queue.emplace("button" ,"broadcaster", false);
  while (!queue.empty()) {
    Pulse p = queue.front();
    queue.pop();
    // std::cout << p.src << (p.level ? " -high-> " : " -low-> ") << p.dst << std::endl;
    ++counts[p.level];
    auto& m = modules[p.dst];
    m.process(p, queue);
  }
  return counts;
}

static Num
solve1(Modules modules) {
  Counts allCounts = {};
  for (int i = 0; i < 1000; ++i) {
    auto counts = pulse(modules);
    allCounts[0] += counts[0];
    allCounts[1] += counts[1];
  }
  return allCounts[0] * allCounts[1];
}

static Num
solve2(Modules modules) {
  Num count = 0;
  Module& rx = modules["rx"];
  while (!rx.receivedLow) {
    pulse(modules);
    ++count;
  }
  return count;
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
      t.inputs[name] = false;
    }
  }

  Num res1 = solve1(modules);
  std::cout << res1 << "\n";

  // Num res2 = solve2(modules);
  // std::cout << res2 << "\n";
}
