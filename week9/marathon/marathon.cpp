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
#include <set>
#include <algorithm>
#include <map>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>


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



// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
        boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > >	Graph2;
// Interior Property Maps
typedef	boost::property_map<Graph2, boost::edge_capacity_t>::type		EdgeCapacityMap2;
typedef	boost::property_map<Graph2, boost::edge_residual_capacity_t>::type	ResidualCapacityMap2;
typedef	boost::property_map<Graph2, boost::edge_reverse_t>::type		ReverseEdgeMap2;
typedef	boost::graph_traits<Graph2>::vertex_descriptor			Vertex2;
typedef	boost::graph_traits<Graph2>::edge_descriptor			Edge2;
typedef	boost::graph_traits<Graph2>::edge_iterator			EdgeIt2;


// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class EdgeAdder {
    Graph2 &G;
    EdgeCapacityMap2	&capacitymap;
    ReverseEdgeMap2	&revedgemap;

public:
    // to initialize the Object
    EdgeAdder(Graph2 & G, EdgeCapacityMap2 &capacitymap, ReverseEdgeMap2 &revedgemap):
            G(G), capacitymap(capacitymap), revedgemap(revedgemap){}

    // to use the Function (add an edge)
    void addEdge(int from, int to, long capacity) {
        Edge2 e, rev_e;
        bool success;
        boost::tie(e, success) = boost::add_edge(from, to, G);
        boost::tie(rev_e, success) = boost::add_edge(to, from, G);
        capacitymap[e] = capacity;
        capacitymap[rev_e] = 0; // reverse edge has no capacity!
        revedgemap[e] = rev_e;
        revedgemap[rev_e] = e;
    }
};
using namespace std;
using namespace boost;



struct street{
    int from;
    int to;
    int length;
    bool operator <(const street& a) const
    {
        return (from < a.from || (a.from == from && to < a.to) || (a.from == from && a.to == to  && length < a.length));
    }
};
void testcase(){

    //read in nr_buyers, nr_sites, nr_states
    int nr_intersections, nr_streets, start, goal;
    cin >> nr_intersections >> nr_streets >> start >> goal;


    // Create Graph and Maps
    Graph G(nr_intersections);
    WeightMap weightmap = boost::get(boost::edge_weight, G);
    map<street, int> capacity_map;

    //read in graph
    for(int i = 0; i < nr_streets; ++i){
        street st;
        int from, to, capacity, length;

        cin >> from >> to >> capacity >> length;

        if(from == to){
            continue;
        }
        //cout << "from " << from << " to " << to << " length " << length << endl;
        if(from < to){
            st.from = from; st.to = to; st.length = length;
        }else{
            st.from = to; st.to = from; st.length = length;
        }
        if(capacity_map.find(st) != capacity_map.end()){
                capacity_map[st] = capacity_map.find(st)->second + capacity;
        }else {
            capacity_map.insert(make_pair(st, capacity));
        }

    }

    //create graph
    for(auto i: capacity_map){
        street st = i.first;
        int capacity = i.second;
        int from = st.from; int to = st.to; int length = st.length;
        Edge e; bool success;
        tie(e, success) = add_edge(from, to, G);
        weightmap[e] = length;
        //cout << "added edge from " << from << " to " << to << " with length " << length <<  endl;
    }



    // Dijkstra shortest paths from start
    // =======================
    std::vector<Vertex> predmap1(nr_intersections);	// We will use this vector as an Exterior Property Map: Vertex -> Dijkstra Predecessor
    std::vector<int> distmap1(nr_intersections);		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
    Vertex s1 = start;
    boost::dijkstra_shortest_paths(G, s1, // We MUST provide at least one of the two maps
                                   boost::predecessor_map(boost::make_iterator_property_map(predmap1.begin(), boost::get(boost::vertex_index, G))).	// predecessor map as Named Parameter
                                           distance_map(boost::make_iterator_property_map(distmap1.begin(), boost::get(boost::vertex_index, G))));	// distance map as Named Parameter



    // Dijkstra shortest paths from end
    // =======================
    std::vector<Vertex> predmap2(nr_intersections);	// We will use this vector as an Exterior Property Map: Vertex -> Dijkstra Predecessor
    std::vector<int> distmap2(nr_intersections);		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
    Vertex s2 = goal;
    boost::dijkstra_shortest_paths(G, s2, // We MUST provide at least one of the two maps
                                   boost::predecessor_map(boost::make_iterator_property_map(predmap2.begin(), boost::get(boost::vertex_index, G))).	// predecessor map as Named Parameter
                                           distance_map(boost::make_iterator_property_map(distmap2.begin(), boost::get(boost::vertex_index, G))));	// distance map as Named Parameter
     int shortestdist = distmap1[goal];
    Graph2 G2(nr_intersections);
    EdgeCapacityMap2 capacitymap = boost::get(boost::edge_capacity, G2);
    ReverseEdgeMap2 revedgemap = boost::get(boost::edge_reverse, G2);
    ResidualCapacityMap2 rescapacitymap = boost::get(boost::edge_residual_capacity, G2);
    EdgeAdder eaG(G2, capacitymap, revedgemap);
     set<int> nodes_on_paths;
        EdgeIt ebeg, eend;
        for (boost::tie(ebeg, eend) = boost::edges(G); ebeg != eend; ++ebeg) {	// edges(G) returns a pair of iterators which define a range of all edges.
        // For undirected graphs, each edge is visited once, with some orientation.
        // ebeg is EdgeIterator, *ebeg is EdgeDescriptor
        Vertex u = source(*ebeg, G);
        Vertex v = target(*ebeg, G);
        //cout << " u " << u << " distmaps u " << distmap1[u] << " " << distmap2[u] << endl;
        //cout << " v " << v << " distmaps v " << distmap1[v] << " " << distmap2[v] << endl;
        if (min(distmap1[u] + distmap2[v], distmap2[u] + distmap1[v]) + weightmap[*ebeg]<= shortestdist){
            street st; st.from = min(u, v); st.to = max(u,v); st.length = weightmap[*ebeg];
            int from, to;
            if(distmap1[u] < distmap1[v]){
                from = u;
                to = v;
            }else{
                from = v;
                to = u;
            }
            eaG.addEdge(from, to, capacity_map[st]);
            //eaG.addEdge(v,u,capacity_map[st]);
            //out << "added edge from " << from << " to " << to << " with weight " << capacity_map[st] << endl;
        }
    }

     long flow1 = boost::push_relabel_max_flow(G2, start,goal);
     cout << flow1 << endl;
}

int main() {
    int nr_testcases;
    cin >> nr_testcases;
    while(nr_testcases--){
        testcase();
    }
    return 0;
}