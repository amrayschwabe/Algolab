#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, directedS, no_property, property<edge_weight_t, int> > Graph;
typedef property_map<Graph, edge_weight_t>::type WeightMap;
typedef graph_traits<Graph>::vertex_descriptor   Vertex;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::edge_iterator EdgeIt;

void testcase() {
  int n,m;
  cin >> n >> m;
  Graph G(n);
  WeightMap wm = get(edge_weight, G);
  for (int i = 0; i < m; i++) {
    int u,v,w;
    cin >> u >> v >> w;
    Edge e; bool success;
    tie(e, success) = add_edge(u, v, G);
    wm[e] = w;
  }

  // find min spanning tree
  vector<Edge> edgeVector;
  kruskal_minimum_spanning_tree(G, back_inserter(edgeVector));

  int sum = 0;
  for (auto edge : edgeVector) {
    sum += wm[edge];
  }

  vector<Vertex> predmap(n);
  vector<int> distmap(n);

  dijkstra_shortest_paths(G,
              0,
              predecessor_map(make_iterator_property_map(predmap.begin(), get(vertex_index, G))).
              distance_map(make_iterator_property_map(distmap.begin(), get(vertex_index, G)))
              );
  int maxDist = -1;
  for (auto dist : distmap) {
    maxDist = max(maxDist, dist);
  }

  cout << sum << " " << maxDist << endl;
}

int main() {
  int t; cin >> t;
  while (t--) {
    testcase();
  }
}
