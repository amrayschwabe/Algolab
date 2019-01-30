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
#include <climits>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/max_cardinality_matching.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace std;
using namespace boost;
typedef boost::adjacency_list<vecS, vecS, undirectedS> my_graph;


// Directed graph with integer weights on edges.
typedef adjacency_list<vecS, vecS, directedS,
no_property,
property<edge_weight_t, int>
>					Graph;
typedef graph_traits<Graph>::vertex_descriptor		Vertex;	// Vertex type
typedef graph_traits<Graph>::edge_descriptor		Edge;	// Edge type
typedef graph_traits<Graph>::edge_iterator		EdgeIt;	// Edge iterator
// Property map edge -> weight
typedef property_map<Graph, edge_weight_t>::type	WeightMap;

void testcase(){
   //read in stuff
   int nr_positions, nr_slopes, nr_agents, nr_shelters, capacity, protocol_time;
   cin >> nr_positions >>  nr_slopes >> nr_agents >> nr_shelters >> capacity >> protocol_time;

    // Create Graph and Maps
    Graph G(nr_positions);
    WeightMap weightmap = boost::get(boost::edge_weight, G);

    // Add the edges
    string sl; int x,y, distance;
    Edge e; bool success;
   for(int i= 0; i < nr_slopes; ++i){
       cin >> sl >> x >> y >> distance;
       if(sl == "L"){
           tie(e,success) = add_edge(x,y,G);
           weightmap[e] = distance;
           tie(e,success) = add_edge(y,x,G);
           weightmap[e] = distance;
       }else{
           tie(e,success) = add_edge(x,y,G);
           weightmap[e] = distance;
       }
   }

    //save agents starting position
    std::vector<vector<int> > d(nr_agents,vector<int>(nr_positions));
    int start;
    for(int i = 0; i < nr_agents; ++i){
        cin >> start;
        dijkstra_shortest_paths(G,start,
                                boost::distance_map(boost::make_iterator_property_map(d[i].begin(), boost::get(boost::vertex_index, G))));
    }


    //read in shelters, create map with distance from every agent to every shelter
    vector<vector<int> >distances(nr_agents, vector<int>(nr_shelters));
    int shelter;
    for(int i = 0; i < nr_shelters; ++i){
        cin >> shelter;
        for(int j = 0; j < nr_agents; ++j){
            distances[j][i] = d[j][shelter];
        }
    }
   //now do binary search over all possible lengths of paths and check for the smallest one where there still exists a
   //path for every agent
   int left = 0; int right = INT_MAX; int middle;
   while(left < right){
       my_graph matching(nr_agents+nr_shelters*capacity);
       std::vector<graph_traits<my_graph>::vertex_descriptor> mate(nr_agents+nr_shelters*capacity);
       //cout << "left " << left << " right " << right << " middle " << middle << endl;
       middle = left + (right - left)/2;
       for(int i = 0; i < nr_agents; ++i){
           for (int j = 0; j < nr_shelters; ++j){
               if(distances[i][j] == INT_MAX){
                   continue;
               }
               for(int c = 0; c < capacity; ++c){
                   //cout << "distances[i][j] + (c+1)*protocol_time " << (distances[i][j] + (c+1)*protocol_time) << endl;
                   //cout << "middle " << middle << endl;
                   if(distances[i][j] + (c+1)*protocol_time <= middle){
                        add_edge(i, nr_agents+c*nr_shelters+j, matching);
                   }
               }
           }
       }
       bool success = checked_edmonds_maximum_cardinality_matching(matching, &mate[0]);
       assert(success);
       int matchingsize = matching_size(matching, &mate[0]);
       //cout << "matchingsize " << matchingsize << " nr agents " << nr_agents << endl;
       if(matchingsize == nr_agents){
           right = middle;
           //cout << "right " << right << endl;
       }else{
           left = middle +1;
           //cout << "left " << left << endl;
       }

   }
   cout << left << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        testcase();
    }

    return 0;
}