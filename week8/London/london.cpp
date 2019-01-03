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
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
#include<string>
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
    //read in height and width
    int height, width;
    cin >> height >> width;
    string goal;
    cin >> goal;
    vector<int>goalcount(26,0);
    for(int i= 0; i< goal.length(); ++i){
        goalcount[goal[i]-'A']++;
    }

	// Create Graph and Maps
	Graph G(26);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

	//read in front and back
	vector<vector<int> >front(height, vector<int>(width));
	for(int i = 0; i<height; ++i){
	    string tmp;
	    cin >> tmp;
	    for(int j = 0; j<width; ++j){
	        front[i][j] = tmp[j]-'A';
	    }
	}
    vector<vector<int> >back(height, vector<int>(width));
    for(int i = 0; i<height; ++i){
        string tmp;
        cin >> tmp;
        for(int j = width-1; j>=0; --j){
            back[i][j] = tmp[width-1-j]-'A';
        }
    }


	//iterate over array of tuples and count how many of each
    vector<vector<int> >frontback(26, vector<int>(26,0));

    for(int i= 0; i<height; ++i){
        for(int j = 0; j<width; ++j){
            int f = front[i][j];
            int b = back[i][j];
        frontback[f][b]++;
        }
    }

    // Add source and sink
    Vertex src = boost::add_vertex(G);
    Vertex sink = boost::add_vertex(G);

    //add vertices for each tuple
    for(int i = 0; i<26; ++i){
        for(int j = 0; j<26;++j){
            Vertex tmp = add_vertex(G);
            eaG.addEdge(src, tmp, frontback[i][j]);
            eaG.addEdge(tmp, i, frontback[i][j]);
            eaG.addEdge(tmp, j, frontback[i][j]);
        }
    }

    //add edges to sink
    for(int i = 0; i<26; ++i){
        eaG.addEdge(i, sink, goalcount[i]);
    }
	// Calculate flow
	// If not called otherwise, the flow algorithm uses the interior properties
	// - edge_capacity, edge_reverse (read access),
	// - edge_residual_capacity (read and write access).
	long flow = boost::push_relabel_max_flow(G, src, sink);
	//long flow2 = boost::edmonds_karp_max_flow(G, source, target);
	//std::cout << "(push relabel max flow) " << flow1 << " == " << flow2 << " (Edmonds Karp max flow)" << std::endl;

    if(flow==goal.length()){
        cout << "Yes" << endl;
    }else{
        cout << "No" << endl;
    }

}

// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
	int T;
	cin >>T;
	for (; T > 0; --T)	testcases();
	return 0;
}
