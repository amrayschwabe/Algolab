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
#include <vector>
#include <map>
#include <climits>

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
using namespace std;
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
const int maxtime = 100000;
void testcase(){

    int nr_requests, nr_rental_stations;
    cin >> nr_requests >> nr_rental_stations;

    //initialize empty graph
    Graph G;
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

    vector<map<int, int> >indexmap(nr_rental_stations+1);
    Vertex source = add_vertex(G);
    Vertex target = add_vertex(G);
    int count = 2;
    int nr_cars = 0;
    //initialize initial car distribution at time 0
    //initialize connection to target
    for(int i = 0; i < nr_rental_stations; ++i){
        int initial_value;
        cin >> initial_value;
        nr_cars+= initial_value;
        eaG.addEdge(source, count, initial_value, 0);
        //cout << "from " << source << " to " << count << " value " << initial_value << endl;
        indexmap[i+1][0] = count;
        count ++;
        eaG.addEdge(count, target, INT_MAX, 0);
        indexmap[i+1][maxtime] = count;
        count ++;
    }

    //cout << "tours " << endl;
    //add tours
    for(int i = 0; i < nr_requests; ++i){
        int start;
        int target;
        int departure;
        int arrival;
        int profit;
        cin >> start >> target >> departure >> arrival >> profit;
        auto departureindex = indexmap[start].find(departure);
        auto arrivalindex = indexmap[target].find(arrival);
        int dindex, arrindex;
        if(departureindex == indexmap[start].end()){
            indexmap[start][departure] = count;
            dindex= count;
            count ++;
        }else{
            dindex = (*departureindex).second;
        }
        if(arrivalindex == indexmap[target].end()){
            indexmap[target][arrival] = count;
            arrindex = count;
            count ++;
        }else{
            arrindex = (*arrivalindex).second;
        }
        eaG.addEdge(dindex, arrindex, 1, (arrival-departure)*100-profit);
        //cout << "from " << dindex << " to " << arrindex << " profit " << profit << endl;
    }
    //cout << " conncetions stations " << endl;
    vector<vector<pair<int, int> > >vectormap(nr_rental_stations);
    for(int i = 1 ; i < nr_rental_stations+1; ++i){
        for(auto it : indexmap[i]){
            vectormap[i-1].push_back(it);
            //cout << "station " << i << " time " << it.first << " index " << it.second << endl;
        }
    }
    //add connections between same station, different time
    for(int i = 0; i < nr_rental_stations; ++i){
        int size = vectormap[i].size();
        for(int j = 0; j < size -1; ++j){
            int time = vectormap[i][j+1].first - vectormap[i][j].first;
            eaG.addEdge(vectormap[i][j].second, vectormap[i][j+1].second, INT_MAX, 100*time);
            //cout << "from " << vectormap[i][j] << " to " << vectormap[i][j+1] << " profit " << 0 << endl;
        }
    }

/*
    // Run the algorithm

    // Option 1: Min Cost Max Flow with cycle_canceling
    int flow1 = boost::push_relabel_max_flow(G, source, target);
    boost::cycle_canceling(G);
    int cost1 = boost::find_flow_cost(G);*/

    // Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights
    boost::successive_shortest_path_nonnegative_weights(G, source, target);
    int cost2 = boost::find_flow_cost(G);
    cout <<(nr_cars*100*maxtime)-cost2 << endl;
}
int main() {

    int t; cin >> t;
    while(t--){
        testcase();
    }
    return 0;
}