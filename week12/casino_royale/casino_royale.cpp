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

using namespace std;
void testcase(){

    int trainstops, nr_missions, limit_agents;
    cin >> trainstops >> nr_missions >> limit_agents;

    // Create Graph and Maps
    Graph G(trainstops+nr_missions);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

    Vertex source = add_vertex(G);
    Vertex sink = add_vertex(G);
    eaG.addEdge(source, 0, limit_agents,0);
    eaG.addEdge(trainstops-1, sink, limit_agents,0);

    for(int i = 0; i < trainstops -1; ++i){
        eaG.addEdge(i, i+1, limit_agents,128);
    }

    for(int i = 0; i < nr_missions; ++i){
        int from, to, priority;
        cin >> from >> to >> priority;
        eaG.addEdge(from, trainstops+i, 1, (to-from)*128-priority);
        eaG.addEdge(trainstops+i, to, 1,0);
    }
/*
    // Option 1: Min Cost Max Flow with cycle_canceling
    int flow1 = boost::push_relabel_max_flow(G, source, sink);
    //cout << flow1 << endl;
    boost::cycle_canceling(G);
    int cost1 = boost::find_flow_cost(G);
    cout << ((trainstops-1)*128*limit_agents-cost1) << endl;
    //std::cout << "-----------------------" << std::endl;
    //std::cout << "Minimum Cost Maximum Flow with cycle_canceling()" << std::endl;
    //std::cout << "flow " << flow1 << std::endl; // 5
    //std::cout << "cost " << cost1 << std::endl; // 12
*/
    // Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights
    boost::successive_shortest_path_nonnegative_weights(G, source, sink);
    int cost2 = boost::find_flow_cost(G);
    cout << ((trainstops-1)*128*limit_agents-cost2) << endl;



}
int main() {
    int t;
    cin >> t;
    while(t--){
        testcase();
    }
    return 0;
}