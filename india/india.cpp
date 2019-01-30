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

using namespace std;
using namespace boost;

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

bool too_small(int cap, EdgeCapacityMap & capacitymap, Graph &G, int source, int goal, Edge e, int total_budget) {
    capacitymap[e] = cap;
    // Option 1: Min Cost Max Flow with cycle_canceling
    boost::successive_shortest_path_nonnegative_weights(G, source, goal);
    int cost1 = boost::find_flow_cost(G);
    if(cost1 <= total_budget){
        //cout << "flow with budget " << total_budget << " and capacity " << cap << " works " << endl;
        //cout << " it has cost " << cost1 << endl;
        return true;
    }else{
        //cout << "flow with budget " << total_budget << " and capacity " << cap << " doesn't work " << endl;
        //cout << " it has cost " << cost1 << endl;
        return false;
    }
}
void testcase(){
    //read in
    int nr_cities, nr_guides, total_budget, start, goal;
    cin >> nr_cities >> nr_guides >> total_budget >> start >> goal;

    // Create Graph and Maps
    Graph G(nr_cities);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

    //read in guides
    for(int i = 0; i < nr_guides; ++i){
        int from, to, cost, capacity;
        cin >> from >> to >> cost >> capacity;
        eaG.addEdge(from, to, capacity, cost);
    }

    //now do binary search over all possible amounts of suitcases

    Vertex source = add_vertex(G);
    Edge e, rev_e;
    boost::tie(e, boost::tuples::ignore) = boost::add_edge(source, start, G);
    boost::tie(rev_e, boost::tuples::ignore) = boost::add_edge(start, source, G);
    capacitymap[e] = INT_MAX;
    weightmap[e] = 0; // new!
    capacitymap[rev_e] = 0;
    weightmap[rev_e] = 0; // new
    revedgemap[e] = rev_e;
    revedgemap[rev_e] = e;


    boost::successive_shortest_path_nonnegative_weights(G, source, goal);
    int cost2 = boost::find_flow_cost(G);
    OutEdgeIt e2, eend;
    int s_flow = 0;
    for(boost::tie(e2, eend) = boost::out_edges(boost::vertex(source,G), G); e2 != eend; ++e2) {
              s_flow += capacitymap[*e2] - rescapacitymap[*e2];
    }
    if(cost2 <= total_budget){
        cout << s_flow << endl;
        return;
    }


    int lmin = 0, lmax = 1;
    while (too_small(lmax,capacitymap, G, source,goal,e, total_budget) && lmax < s_flow) lmax *= 2;
    while (lmin != lmax) {
        int p = (lmin + lmax)/2;
        if (too_small(p, capacitymap, G, source, goal,e, total_budget))
            lmin = p + 1;
        else
            lmax = p;
    }
    int L = lmin-1;
    capacitymap[e] = L;
    boost::successive_shortest_path_nonnegative_weights(G, source,goal);
    int result = 0;
    // Iterate over all edges leaving the source to sum up the flow values.
    OutEdgeIt e3, eend3;
    for(boost::tie(e3, eend3) = boost::out_edges(boost::vertex(source,G), G); e3 != eend3; ++e3) {
        result += capacitymap[*e3] - rescapacitymap[*e3];
    }

    cout << result << endl;
    // Run the algorithm

/*
//check if 1 is okay
    capacitymap[e] = 1;
    boost::successive_shortest_path_nonnegative_weights(G, source, goal);
    int cost2 = boost::find_flow_cost(G);
    if(cost2 <= total_budget){
        cout << 1 << endl;
    }else{
        cout << 0 << endl;
    }*/
/*
    // Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights
    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
    int cost2 = boost::find_flow_cost(G);
    std::cout << "-----------------------" << std::endl;
    std::cout << "Minimum Cost Maximum Flow with successive_shortest_path_nonnegative_weights()" << std::endl;
    std::cout << "cost " << cost2 << std::endl; // 12
    int s_flow = 0;
    // Iterate over all edges leaving the source to sum up the flow values.
    OutEdgeIt e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
        std::cout << "edge from " << boost::source(*e, G) << " to " << boost::target(*e, G)
                  << " with capacity " << capacitymap[*e] << " and residual capacity " << rescapacitymap[*e] << std::endl;
        s_flow += capacitymap[*e] - rescapacitymap[*e];
    }
    std::cout << "s-out flow " << s_flow << std::endl; // 5
    // Or equivalently, you can do the summation at the sink, but with reversed sign.
    int t_flow = 0;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_target,G), G); e != eend; ++e) {
        std::cout << "edge from " << boost::source(*e, G) << " to " << boost::target(*e, G)
                  << " with capacity " << capacitymap[*e] << " and residual capacity " << rescapacitymap[*e] << std::endl;
        t_flow += rescapacitymap[*e] - capacitymap[*e];
    }
    std::cout << "t-in flow " << t_flow << std::endl; // 5
    std::cout << "-----------------------" << std::endl;
*/
}
int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        testcase();
    }

    return 0;
}