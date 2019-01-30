// ALGOLAB BGL Tutorial 3
// Flow example demonstrating
// - breadth first search (BFS) on the residual graph

// Compile and run with one of the following:
// g++ -std=c++11 -O2 bgl_residual_bfs.cpp -o bgl_residual_bfs ./bgl_residual_bfs
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 bgl_residual_bfs.cpp -o bgl_residual_bfs; ./bgl_residual_bfs

// Includes
// ========
// STL includes
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <climits>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace std;
using namespace boost;

// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
        boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > >	Graph;
// Interior Property Maps
typedef	boost::property_map<Graph, boost::edge_capacity_t>::type		EdgeCapacityMap;
typedef	boost::property_map<Graph, boost::edge_residual_capacity_t>::type	ResidualCapacityMap;
typedef	boost::property_map<Graph, boost::edge_reverse_t>::type		ReverseEdgeMap;
typedef	boost::graph_traits<Graph>::vertex_descriptor			Vertex;
typedef	boost::graph_traits<Graph>::edge_descriptor			Edge;
typedef	boost::graph_traits<Graph>::out_edge_iterator			OutEdgeIt;

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class EdgeAdder {
    Graph &G;
    EdgeCapacityMap	&capacitymap;
    ReverseEdgeMap	&revedgemap;

public:
    // to initialize the Object
    EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap):
            G(G), capacitymap(capacitymap), revedgemap(revedgemap){}

    // to use the Function (add an edge)
    void addEdge(int from, int to, long capacity) {
        Edge e, rev_e;
        bool success;
        boost::tie(e, success) = boost::add_edge(from, to, G);
        boost::tie(rev_e, success) = boost::add_edge(to, from, G);
        capacitymap[e] = capacity;
        capacitymap[rev_e] = 0;
        revedgemap[e] = rev_e;
        revedgemap[rev_e] = e;
    }
};

void testcase(){

    //read in nr_figures, nr_limbs
    int nr_figures, nr_limbs;
    cin >> nr_figures >> nr_limbs;

    // build Graph
    Graph G(nr_figures);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, revedgemap);


    // add edges
    for(int i = 0; i < nr_limbs; ++i){
        int from, to, cost;
        cin >> from >> to >> cost;
        eaG.addEdge(from, to,  cost);
    }

    int bestsink;
    int bestsource;
    int bestflow = INT_MAX;
    int flow;
    //try every edge once
    for(int i = 1; i < nr_figures; ++i){
        //as source
        flow = push_relabel_max_flow(G, i, 0);
        //cout <<"from "<< i << " to " << 0 << "flow" << flow << endl;
        if(flow < bestflow) {
            bestflow = flow;
            bestsink = 0;
            bestsource = i;
        }
        //as sink
        flow = push_relabel_max_flow(G, 0, i);
        //cout <<"from "<< 0 << " to " << i << "flow" << flow << endl;
        if(flow < bestflow){
            bestflow = flow;
            bestsink = i;
            bestsource = 0;
        }
    }

    // Find a min cut via maxflow
    flow = boost::push_relabel_max_flow(G, bestsource, bestsink);
    //nr of figures to take home
    int nr_to_take_home = 1;
    // BFS to find vertex set S
    std::vector<int> vis(nr_figures, false); // visited flags
    std::queue<int> Q; // BFS queue (from std:: not boost::)
    vis[bestsource] = true; // Mark the source as visited
    Q.push(bestsource);
    while (!Q.empty()) {
        const int u = Q.front();
        Q.pop();
        OutEdgeIt ebeg, eend;
        for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
            const int v = boost::target(*ebeg, G);
            // Only follow edges with spare capacity
            if (rescapacitymap[*ebeg] == 0 || vis[v]) continue;
            vis[v] = true;
            nr_to_take_home ++;
            Q.push(v);
        }
    }

    //output cost of optimal cut
    cout << flow << endl;

    //output nr to take home
    cout << nr_to_take_home << " ";
    // Output ground ones

    for (int i = 0; i < nr_figures; ++i) {
        if (vis[i]){
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;

}
// Main
int main() {
    int nr;
    cin >> nr;
    while(nr--){
        testcase();
    }
}