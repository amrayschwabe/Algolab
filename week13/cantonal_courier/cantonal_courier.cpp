// ALGOLAB BGL Tutorial 2
// Flow example demonstrating
// - interior graph properties for flow algorithms
// - custom edge adder

// Compile and run with one of the following:
// g++ -std=c++11 -O2 flows.cpp -o flows ./flows
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 flows.cpp -o flows; ./flows

// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
// Namespaces
// using namespace std;
// using namespace boost;


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
typedef	boost::graph_traits<Graph>::edge_iterator			EdgeIt;

using namespace std;

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
        capacitymap[rev_e] = 0; // reverse edge has no capacity!
        revedgemap[e] = rev_e;
        revedgemap[rev_e] = e;
    }
};


// Functions
// =========
// Function for an individual testcase
void testcases() {

    //read in
    int nr_zones, nr_jobs;
    cin >> nr_zones >> nr_jobs;

    // Create Graph and Maps
    Graph G(nr_jobs + nr_zones);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, revedgemap);

    Vertex sink = add_vertex(G);
    Vertex source = add_vertex(G);
    //read in zonecost and add to graoh
    for(int i= 0; i < nr_zones; ++i){
        int cost; cin >> cost;
        eaG.addEdge(i, sink, cost);
    }

    //read in rewards and add to graph
    int totalreward = 0;
    for(int i = 0; i < nr_jobs; ++i){
        int reward; cin >> reward;
        eaG.addEdge(source, nr_zones + i, reward);
        totalreward += reward;
    }

    //read in which zones belong to which job
    for(int i = 0; i < nr_jobs; ++i){
        int zones; cin >> zones;
        for(int j = 0; j < zones; ++j){
            int zonenumber; cin >> zonenumber;
            eaG.addEdge(nr_zones+i, zonenumber, INT_MAX);
        }
    }


    // Calculate flow
    // If not called otherwise, the flow algorithm uses the interior properties
    // - edge_capacity, edge_reverse (read access),
    // - edge_residual_capacity (read and write access).
    long flow1 = boost::push_relabel_max_flow(G, source, sink);

    cout << (totalreward-flow1) << endl;

}

// Main function to loop over the testcases
int main() {
    std::ios_base::sync_with_stdio(false);
    int T;	cin >> T;
    for (; T > 0; --T)	testcases();
    return 0;
}