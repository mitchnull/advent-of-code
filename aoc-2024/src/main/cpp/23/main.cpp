#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_set>
#include <iterator>

using Num = int;

using String = std::string;
using Strings = std::vector<String>;
using Pair = std::pair<String, String>;
using Set = std::unordered_set<String>;
using Pairs = std::vector<Pair>;

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

static void
findCluster(Strings& cluster, const Set& edges, String from, Set& visited) {
  cluster.push_back(from);
  visited.insert(from);
  for (auto e : edges) {
    if (e.starts_with(from) && !visited.contains(v2(e))) {
      findCluster(cluster, edges, v2(e), visited);
    }
  }
}

static std::vector<Strings>
findClustersWithT(const Set& edges) {
  Set visited;
  std::vector<Strings> clusters;
  for (auto eb = edges.begin(), ee = edges.end(); eb != ee; ++eb) {
    if (eb->front() == 't' && !visited.contains(v1(*eb))) {
      Strings cluster;
      findCluster(cluster, edges, v1(*eb), visited);
      clusters.push_back(cluster);
    }
  }
  return clusters;
}

static Num
solve1(const Strings& cluster) {
  Num res = 0;
  for (Num ts = std::count_if(cluster.begin(), cluster.end(), [](auto v) { return v.front() == 't'; }), size = cluster.size();
      ts > 0 && size >=3;
      --ts, --size) {
    res += ((size - 1) * (size - 2)) / 2;
  }
  return res;
}

int
main() {
  Pairs pairs;
  String line;
  while (std::cin >> line) {
    pairs.emplace_back(line.substr(0, 2), line.substr(3));
  }
  Set edges;
  std::transform(pairs.begin(), pairs.end(), std::inserter(edges, edges.begin()), [](auto p) { return toEdge(p); });
  std::transform(pairs.begin(), pairs.end(), std::inserter(edges, edges.begin()), [](auto p) { return toEdge(rev(p)); });

  Set visited;
  auto tClusters = findClustersWithT(edges);
  for (auto tc : tClusters) {
    std::cout << tc << std::endl;
  }
  Num res1 = std::transform_reduce(tClusters.begin(), tClusters.end(), Num{}, std::plus<>(), solve1);

  std::cout << "1: " << res1 << std::endl;
}
  
