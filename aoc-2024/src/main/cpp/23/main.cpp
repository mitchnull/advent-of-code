#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <unordered_map>
#include <vector>

using Num = int;

using String = std::string;
using Strings = std::vector<String>;
using Pair = std::pair<String, String>;
using Pairs = std::vector<Pair>;
using Neighbors = std::unordered_map<String, Strings>;

/* ------------------------------------------------------------------------ */

static Num
solve1(const Neighbors& neighbors) {
  Strings cliques;
  for (auto& [a, ats] : neighbors) {
    if (a.front() == 't') {
      for (auto b : ats) {
        for (auto c : neighbors.at(b)) {
          auto& cts = neighbors.at(c);
          if (std::find(cts.begin(), cts.end(), a) != cts.end()) {
            std::array<String, 3> clique = {a, b, c};
            std::sort(clique.begin(), clique.end());
            cliques.push_back(clique[0] + clique[1] + clique[2]);
          }
        }
      }
    }
  }
  std::sort(cliques.begin(), cliques.end());
  return std::unique(cliques.begin(), cliques.end()) - cliques.begin();
}

/* ------------------------------------------------------------------------ */

static Strings
bronKerboschIsh(const Neighbors& neighbors, const Strings& r, const Strings& p, Strings maxClique) {
  if (p.empty()) {
    return r.size() > maxClique.size() ? r : maxClique;
  }
  for (auto it = p.begin(), end = p.end(); it != end; ++it) {
    const auto& n = neighbors.at(*it);
    Strings rr;
    std::set_union(r.begin(), r.end(), it, it + 1, std::back_inserter(rr));
    Strings pp;
    std::set_intersection(it, p.end(), n.begin(), n.end(), std::back_inserter(pp));
    auto mc = bronKerboschIsh(neighbors, rr, pp, maxClique);
    if (mc.size() > maxClique.size()) {
      std::swap(maxClique, mc);
    }
  }
  return maxClique;
}

static String
solve2(const Strings& vertices, const Neighbors& neighbors) {
  Strings maxClique = bronKerboschIsh(neighbors, {}, vertices, {});
  return std::accumulate(std::next(maxClique.begin()), maxClique.end(), *maxClique.begin(), [](auto s, auto n) { s += ','; return s += n; });
}

/* ------------------------------------------------------------------------ */

int
main() {
  Neighbors neighbors;
  Strings vertices;
  String line;
  while (std::cin >> line) {
    auto f = line.substr(0, 2);
    auto t = line.substr(3);
    vertices.push_back(f);
    vertices.push_back(t);
    neighbors[f].push_back(t);
    neighbors[t].push_back(f);
  }
  std::sort(vertices.begin(), vertices.end());
  vertices.erase(std::unique(vertices.begin(), vertices.end()), vertices.end());
  for (auto& [f, ts] : neighbors) {
    std::sort(ts.begin(), ts.end());
  }

  std::cout << "1: " << solve1(neighbors) << std::endl;
  std::cout << "2: " << solve2(vertices, neighbors) << std::endl;
}
