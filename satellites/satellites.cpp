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
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

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

    //read in ground stations, satellites,links
    int g, s, l;
    cin >> g >> s >> l;

    // build Graph
    Graph G(g+s);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, revedgemap);

    Vertex src = add_vertex(G);
    Vertex sink = add_vertex(G);

    // add edges
    for(int i = 0; i < l; ++i){
        int from, to;
        cin >> from >> to;
        eaG.addEdge(from, to+g,  1);
    }

    //add edges from source
    for(int i = 0; i < g; ++i){
        eaG.addEdge(src, i, 1);
    }

    //add edges to sink
    for(int i = 0; i<s; ++i){
        eaG.addEdge(i+g, sink, 1);
    }

    // Find a min cut via maxflow
    int flow = boost::push_relabel_max_flow(G, src, sink);

    // BFS to find vertex set S
    std::vector<int> vis(s+g+2, false); // visited flags
    std::queue<int> Q; // BFS queue (from std:: not boost::)
    vis[src] = true; // Mark the source as visited
    Q.push(src);
    while (!Q.empty()) {
        const int u = Q.front();
        Q.pop();
        OutEdgeIt ebeg, eend;
        for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
            const int v = boost::target(*ebeg, G);
            // Only follow edges with spare capacity
            if (rescapacitymap[*ebeg] == 0 || vis[v]) continue;
            vis[v] = true;
            Q.push(v);
        }
    }


    int g_ = 0;
    int s_ = 0;
    //BFS to find MIN Vertex cover
    std::vector<int> vis2(s+g+2, false); // visited flags
    std::vector<int> visg(s+g+2, false); // visited flags ground
    std::vector<int> viss(s+g+2, false); // visited flags satellites
    std::queue<int> Q2; // BFS queue (from std:: not boost::)
    Q2.push(sink);
    while (!Q2.empty()) {
        const int u = Q2.front();
        Q2.pop();
        OutEdgeIt ebeg, eend;
        for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
            const int v = boost::target(*ebeg, G);
            const int sr = boost::source(*ebeg, G);
            if(vis2[v] == true){
                continue;
            }
            if(vis[v] == true){
                if(v == src){
                    visg[sr] = true;
                    g_++;
                    continue;
                }
                if(v > g-1){
                    s_++;
                    viss[v] = true;
                }
                else{
                    g_++;
                    visg[v] = true;
                }
                vis2[v] = true;
                continue;
            }
            vis2[v] = true;
            Q2.push(v);
        }
    }

    //output g_ and s_
    cout << g_ << " " << s_ << endl;
    // Output ground ones
    for (int i = 0; i < g; ++i) {
        if (visg[i]){
            std::cout << i << " ";
        }
    }

    //output satellites
    for (int i = g; i < g+s; ++i) {
        if (viss[i]){
            std::cout << i-g << " ";
        }
    }
    if(g_ + s_ > 0) {
        std::cout << std::endl;
    }
}
// Main
int main() {
    int nr;
    cin >> nr;
    while(nr--){
        testcase();
    }
}