// ALGOLAB BGL Tutorial 3
// Code snippets demonstrating
// - MinCostMaxFlow with arbitrary edge costs using cycle_canceling
// - MinCostMaxFlow with non-negative edge costs using successive_shortest_path_nonnegative_weights

// Compile and run with one of the following:
// g++ -std=c++11 -O2 bgl_mincostmaxflow.cpp -o bgl_mincostmaxflow; ./bgl_mincostmaxflow
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 bgl_mincostmaxflow.cpp -o bgl_mincostmaxflow; ./bgl_mincostmaxflow

// Includes
// ========
// STL includes
#include <iostream>
#include <cstdlib>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
        boost::property<boost::edge_reverse_t, Traits::edge_descriptor,
        boost::property <boost::edge_weight_t, long> > > > > Graph; // new!
// Interior Property Maps
typedef boost::property_map<Graph, boost::edge_capacity_t>::type      EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_weight_t >::type       EdgeWeightMap; // new!
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type ResidualCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type       ReverseEdgeMap;
typedef boost::graph_traits<Graph>::vertex_descriptor          Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor            Edge;
typedef boost::graph_traits<Graph>::out_edge_iterator  OutEdgeIt; // Iterator

using namespace std;
using namespace boost;

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map, weight map and reverse edge map
// ===============================================================
class EdgeAdder {
    Graph &G;
    EdgeCapacityMap &capacitymap;
    EdgeWeightMap &weightmap;
    ReverseEdgeMap  &revedgemap;

public:
    EdgeAdder(Graph &G, EdgeCapacityMap &capacitymap, EdgeWeightMap &weightmap, ReverseEdgeMap &revedgemap)
            : G(G), capacitymap(capacitymap), weightmap(weightmap), revedgemap(revedgemap) {}

    void addEdge(int u, int v, long c, long w) {
        Edge e, rev_e;
        boost::tie(e, boost::tuples::ignore) = boost::add_edge(u, v, G);
        boost::tie(rev_e, boost::tuples::ignore) = boost::add_edge(v, u, G);
        capacitymap[e] = c;
        weightmap[e] = w; // new!
        capacitymap[rev_e] = 0;
        weightmap[rev_e] = -w; // new
        revedgemap[e] = rev_e;
        revedgemap[rev_e] = e;
    }
};
void testcase(){

    //read in nr_buyers, nr_sites, nr_states
    int nr_buyers, nr_sites, nr_states;
    cin >> nr_buyers >> nr_sites >> nr_states;

    // Create Graph and Maps
    Graph G(nr_buyers+nr_states+nr_sites);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

    Vertex src = add_vertex(G);
    Vertex sink = add_vertex(G);

    //read in limits for number of sites per state and add to graph
    for(int i = 0; i<nr_states; ++i){
        int limit;
        cin >> limit;
        eaG.addEdge(src, i, limit, 0);
        //cout << "from " << src << " to " << i << " cap " << limit << " cost " << 0 << endl;
    }

    //read in sites for the different states and add to graph
    for(int i = 0; i<nr_sites; ++i){
        int state;
        cin >> state;
        eaG.addEdge(state-1, i +nr_states, 1, 0);
        //cout << "from " << state-1 << " to " << i +nr_states << " cap " << 1 << " cost " << 0 << endl;
    }

    //read in buyers and add to graph
    for(int i = 0; i < nr_buyers; ++i){
        for(int j = 0; j <nr_sites; ++j){
            int bet;
            cin >> bet;
            eaG.addEdge(nr_states+j, nr_states+nr_sites+i, 1, 100-bet);
            //cout << "from " << nr_states+j << " to " << nr_states+nr_sites+i << " cap " << 1 << " cost " << 100-bet << endl;
        }
    }

    //connect edges to sink
    for(int i = 0; i<nr_buyers; ++i){
        eaG.addEdge(nr_states+nr_sites+i, sink, 1, 0);
        //cout << "from " << nr_states+nr_sites+i << " to " << sink << " cap " << 1 << " cost " << 0 << endl;

    }


    // Run the algorithm
/*
    // Option 1: Min Cost Max Flow with cycle_canceling
    int flow1 = boost::push_relabel_max_flow(G, v_source, v_target);
    boost::cycle_canceling(G);
    int cost1 = boost::find_flow_cost(G);
    std::cout << "-----------------------" << std::endl;
    std::cout << "Minimum Cost Maximum Flow with cycle_canceling()" << std::endl;
    std::cout << "flow " << flow1 << std::endl; // 5
    std::cout << "cost " << cost1 << std::endl; // 12
*/
    // Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights
    boost::successive_shortest_path_nonnegative_weights(G, src, sink);
    int cost = boost::find_flow_cost(G);
    int s_flow = 0;
    // Iterate over all edges leaving the source to sum up the flow values.
    OutEdgeIt e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(src,G), G); e != eend; ++e) {
        //std::cout << "edge from " << boost::source(*e, G) << " to " << boost::target(*e, G)
                 // << " with capacity " << capacitymap[*e] << " and residual capacity " << rescapacitymap[*e] << std::endl;
              s_flow += capacitymap[*e] - rescapacitymap[*e];
    }

    //calculate profit = nr of sites * 100 - cost
    int profit = s_flow *100 - cost;
    cout << s_flow << " " << profit << endl;
}
int main() {
    int nr_testcases;
    cin >> nr_testcases;
    while(nr_testcases--){
        testcase();
    }
    return 0;
}