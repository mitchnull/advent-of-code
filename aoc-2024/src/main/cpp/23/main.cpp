#include <iostream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <iterator>
#include <unordered_map>

using Num = int;

using String = std::string;
using Strings = std::vector<String>;
using Pair = std::pair<String, String>;
using Pairs = std::vector<Pair>;
using Neighbors = std::unordered_map<String, Strings>;

template <typename T>
static std::ostream&
operator<<(auto& os, const std::vector<T>& v) {
  os << "{";
  std::string sep;
  for (auto e : v) {
    os << sep << e;
    sep = ", ";
  }
  return os << "}";
}

static constexpr String
toEdge(Pair p) {
  return p.first + p.second;
}

static constexpr Pair
rev(Pair p) {
  return {p.second, p.first};
}

static constexpr String
v1(String edge) {
  return edge.substr(0, 2);
}

static constexpr String
v2(String edge) {
  return edge.substr(2);
}

static Num
solve1(const Strings& edges) {
  Strings cliques;
  for (auto a : edges) {
    if (a.front() == 't') {
      for (auto b : edges) {
        if (v1(b) == v2(a)) {
          for (auto c : edges) {
            if (v1(c) == v2(b) && v2(c) == v1(a)) {
              std::array<String, 3> clique = {v1(a), v1(b), v1(c)};
              std::sort(clique.begin(), clique.end());
              cliques.push_back(clique[0] + clique[1] + clique[2]);
            }
          }
        }
      }
    }
  }
  std::sort(cliques.begin(), cliques.end());
  return std::unique(cliques.begin(), cliques.end()) - cliques.begin();
}

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
solve2(const Strings& edges) {
  Neighbors neighbors;
  Strings vertices;
  for (auto e : edges) {
    auto v = v1(e);
    if (vertices.empty() || vertices.back() != v) {
      vertices.push_back(v);
    }
    neighbors[v].push_back(v2(e));
  }
  Strings maxClique = bronKerboschIsh(neighbors, {}, vertices, {});
  return std::accumulate(std::next(maxClique.begin()), maxClique.end(), *maxClique.begin(), [](auto s, auto n) { s += ','; return s += n; });
}

int
main() {
  Pairs pairs;
  String line;
  while (std::cin >> line) {
    pairs.emplace_back(line.substr(0, 2), line.substr(3));
  }
  Strings edges;
  std::transform(pairs.begin(), pairs.end(), std::inserter(edges, edges.begin()), [](auto p) { return toEdge(p); });
  std::transform(pairs.begin(), pairs.end(), std::inserter(edges, edges.begin()), [](auto p) { return toEdge(rev(p)); });
  std::sort(edges.begin(), edges.end());
  edges.erase(std::unique(edges.begin(), edges.end()), edges.end());

  std::cout << "1: " << solve1(edges) << std::endl;
  std::cout << "2: " << solve2(edges) << std::endl;
}
