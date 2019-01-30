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


// Functions
// =========
// Function for an individual testcase
void testcases() {
    int nr_players, nr_rounds;
    cin >> nr_players >> nr_rounds;

    // Create Graph and Maps
    Graph G(nr_players+nr_rounds);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, revedgemap);


    Vertex src = boost::add_vertex(G);
    Vertex sink = boost::add_vertex(G);
    vector<int> score(nr_players,0);
    int f, t, w;
    int matches = 0;
    for(int i = 0; i<nr_rounds; ++i){
        cin >> f >> t >> w;
        if(w ==0) {
            eaG.addEdge(src,i,1);
            eaG.addEdge(i, nr_rounds + f, 1);
            eaG.addEdge(i, nr_rounds + t, 1);
            matches ++;
        }
        else if(w == 1){
            //cout << "ff " << f <<  " t " << t << endl;
            score[f]--;
        }
        else{
            score[t]--;
            //cout << "f " << f <<  " tt " << t << endl;
        }
    }
    long s;
    int impossible = 0;
    long sumscores = 0;
    for(int i=0; i<nr_players; ++i){
        //cout <<"i " <<  i << " score " << score[i]<<endl;
        cin >> s;
        //cout << "s " << s << endl;
        score[i] += s;
        sumscores+=score[i];
        if(score[i] <0){
            impossible =1;
        }
        eaG.addEdge(nr_rounds + i, sink, score[i]);
        //cout <<"i " <<  i << " score " << score[i]<<endl;
    }
    if(sumscores != matches){
        impossible = 1;
    }
    if(impossible == 1){
        cout << "no" << endl;
        return;
    }

/*
    EdgeIt ebeg, eend;
    for (tie(ebeg, eend) = edges(G); ebeg != eend; ++ebeg) {
        std::cout << "edge from " << boost::source(*ebeg, G) << " to " << boost::target(*ebeg, G)
                  << " runs " << capacitymap[*ebeg]
                  << " units of flow (negative for reverse direction)." << std::endl;
    }*/


  // Calculate flow
  // If not called otherwise, the flow algorithm uses the interior properties
  // - edge_capacity, edge_reverse (read access),
  // - edge_residual_capacity (read and write access).
    long flow = push_relabel_max_flow(G, src, sink);
    //long flow = boost::edmonds_karp_max_flow(G, src, sink);
    //cout << "flow " << flow << endl;
    //cout << "sumscores " << sumscores<< endl;
    // Iterate over all the edges to print the flow along them
    /*
    EdgeIt ebeg, eend;
    for (tie(ebeg, eend) = edges(G); ebeg != eend; ++ebeg) {
        std::cout << "edge from " << boost::source(*ebeg, G) << " to " << boost::target(*ebeg, G)
                  << " runs " << capacitymap[*ebeg] - rescapacitymap[*ebeg]
                  << " units of flow (negative for reverse direction)." << std::endl;
    }*/
    if(flow==sumscores){
        cout << "yes" << endl;
    }else{
        cout <<"no" << endl;
    }


}

// Main function to loop over the testcases
int main() {
    std::ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while(t--)	testcases();
    return 0;
}