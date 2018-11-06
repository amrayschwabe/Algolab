#include <iostream>
#include <vector>
#include <iostream>
#include <string>
#include <boost/graph/push_relabel_max_flow.hpp>
#include<boost/graph/adjacency_list.hpp>


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
using namespace std;
using namespace boost;


void testcase(){


    int nr_locations, nr_paths;
    cin >> nr_locations >> nr_paths;

    // Create Graph and Maps
    Graph G(nr_locations+2);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, revedgemap);


    Vertex src = nr_locations;
    Vertex sink = nr_locations+1;
    vector<pair<int,int> > soldiers(nr_locations);
    int soldiers_curr, soldiers_needed;
    for(int i = 0; i< nr_locations; ++i){
        cin >> soldiers_curr >> soldiers_needed;
        soldiers[i].first = soldiers_curr;
        soldiers[i].second = soldiers_needed;
    }
    //vector<vector<pair<int, int> > > pathvec(nr_paths, vector<pair<int,int> > (nr_paths, make_pair(-1,-1)));
    int from, to, min, max;
    for(int i = 0; i < nr_paths; ++i){
        cin >> from >> to >> min >> max;
        soldiers[from].second +=  min;
        soldiers[to].second -= min;
        eaG.addEdge(from, to, max-min);
    }
    int sumflow=0;
    for(int i = 0; i < nr_locations; ++i){
        eaG.addEdge(src, i, soldiers[i].first);
        if(soldiers[i].second > 0) {
            eaG.addEdge(i, sink, soldiers[i].second);
            sumflow += soldiers[i].second;
        }
    }
    //int flow = edmonds_karp_max_flow(G, src, sink);
    int flow = push_relabel_max_flow(G, src, sink);


    if(flow != sumflow){
        cout << "no" << endl;
        //cout << "flow "<< flow << endl;
        //cout << "sumflow " << sumflow << endl << endl;

    }
    else{
        cout << "yes" << endl;
        //cout << "flow "<< flow << endl;
        //cout << "sumflow " << sumflow << endl << endl;
    }
    return;



}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        testcase();
    }
}