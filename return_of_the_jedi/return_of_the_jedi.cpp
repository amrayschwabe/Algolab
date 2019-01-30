// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <queue>
#include <climits>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
boost::no_property,				// interior properties of vertices
boost::property<boost::edge_weight_t, int> 		// interior properties of edges
>					Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef boost::graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t

using namespace std;

using namespace boost;

void testcase() {
	int n, start_index;
	cin >> n >> start_index;

    //read in nodes into a graph
	Graph G(n);
	WeightMap weightmap = boost::get(boost::edge_weight, G);

	for (int j = 1; j < n; ++j) {
		for (int k = 1; k <= n - j; ++k) {
			int cost; cin >> cost;
			Edge e;	bool success;
			boost::tie(e, success) = boost::add_edge(j - 1, j + k - 1, G);
			weightmap[e] = cost;
		}
	}
	//calculate minimum spanning tree
	std::vector<Vertex> primpredmap(n);
	boost::prim_minimum_spanning_tree(G, boost::make_iterator_property_map(primpredmap.begin(),
																		   boost::get(boost::vertex_index, G)), boost::root_vertex(start_index-1));
	//create graph from minimum spanning tree
	Graph MST(n);
	WeightMap w = boost::get(boost::edge_weight, MST);

	int totalweight = 0;
	for (int i = 0; i < n; ++i) {
		Edge e; bool success;
		Edge eold;
		if(i!=primpredmap[i]) {
			tie(e, success) = add_edge(i, primpredmap[i], MST);
			tie(eold, success) = edge(i, primpredmap[i], G);
			w[e] = boost::get(edge_weight_t(), G, eold);
			totalweight += w[e];
		}
	}

	//for every two nodes, calculate max distance on path
	//do so by doing bfs for every node
	//and for every node take max(weight[node, pred[node]], maxtable[start,pred[node]])
	vector<vector<int> > maxCost(n, vector<int>(n, 0));
	for (int u = 0; u < n; ++u) {
		boost::queue<int> Q;
		Q.push(u);
		vector<bool> vis(n,false);
		vis[u] = true;
		while(!Q.empty()) {
			const int x = Q.front();
			Q.pop();
			OutEdgeIt ebeg, eend;
			for (tie(ebeg, eend) = out_edges(x, MST); ebeg != eend; ++ebeg) {
				const int v = target(*ebeg, MST);
				if(vis[v]){
					continue;
				}
				maxCost[u][v] = max(maxCost[u][x],w[*ebeg]);
				maxCost[v][u] = maxCost[u][v];
				vis[x] = true;
				Q.push(v);
			}
		}
	}

    //iterate over all edges and minimize weight(edge) - maxtable[start, end]
	EdgeIt ebeg, eend;
	int min_additional_weight = INT_MAX;
	for (tie(ebeg, eend) = edges(G); ebeg != eend; ++ebeg) {
		Edge e; bool success;
		if((primpredmap[target(*ebeg, G)] == source(*ebeg, G))|| (primpredmap[source(*ebeg, G)] == target(*ebeg, G)))
		{
			continue;
		}
			int edgeweight = weightmap[*ebeg];
			int max_weight_on_cycle = maxCost[source(*ebeg, G)][target(*ebeg, G)];
			min_additional_weight = min(min_additional_weight, (edgeweight - max_weight_on_cycle));

	}
	cout << (totalweight+min_additional_weight)<< endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t; cin >> t;
	while(t--)
		testcase();
}