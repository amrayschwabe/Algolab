#include <iostream>
#include <vector>
#include <climits>

#include <boost/config.hpp>
#include <iostream>
#include <fstream>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/strong_components.hpp>


using namespace std;
using namespace boost;

typedef adjacency_list < listS, vecS, directedS,
        no_property, property < edge_weight_t, int > > graph_t;
typedef graph_traits < graph_t >::vertex_descriptor vertex_descriptor;
typedef std::pair<int, int> Edge;

void testcase(){
    //read in
    int nrplanets, nredges, nrwarehouses, nrteleport;
    cin >> nrplanets >> nredges >> nrwarehouses >> nrteleport;
    set<int> teleport;
    for(int i = 0; i < nrteleport; i++) {
        int tmp;
        cin >> tmp;
        teleport.insert(tmp);
    }
    Edge edgevector[nredges];
    int weights[nredges];
    for(int i = 0; i<nredges; i++){
        int u,v,w;
        cin >> u >> v >> w;
        edgevector[i] = Edge(v,u);
        weights[i] = w;
    }

    //create graph
    graph_t g(edgevector, edgevector + nredges, weights, nrplanets);
    property_map<graph_t, edge_weight_t>::type weightmap = get(edge_weight, g);
    std::vector<vertex_descriptor> p(num_vertices(g));
    std::vector<int> d(num_vertices(g));

    //compute strong components to find out which parts are fully connected
    vector<int> component(nrplanets);
    int nr_components = strong_components(g, make_iterator_property_map(component.begin(), get(vertex_index, g)));

    //for each component, add list of all members
    vector<vector<int>> reachable(nr_components);
    for (auto t : teleport) {
        reachable[component[t]].push_back(t);
    }

    //add teleport for all in connected components
    for(auto l:reachable){
        vertex_descriptor u = add_vertex(g);
        int size = l.size()-1;
        for(vertex_descriptor q:l){
            add_edge(u,q,size,g);
            add_edge(q,u,0,g);
        }
    }

    weightmap = get(edge_weight, g);
    std::vector<vertex_descriptor> p1(num_vertices(g));
    std::vector<int> d1(num_vertices(g));

    //dijkstra
    dijkstra_shortest_paths(g, nrplanets-1,
            predecessor_map(boost::make_iterator_property_map(p1.begin(), get(boost::vertex_index, g))).
            distance_map(boost::make_iterator_property_map(d1.begin(), get(boost::vertex_index, g))));
    int min = INT_MAX;

    //find closest warehouse
    for(int i = 0; i < nrwarehouses; i++){
        if(min > d1[i]){
            min = d1[i];
        }
    }
    if(min > 1000000){
        cout << "no" << endl;
    }else{
        cout << min << endl;
    }

};

int main() {
    std::ios_base::sync_with_stdio(false);
    int n;
    cin >> n;
    while(n--){
        testcase();
    }
}