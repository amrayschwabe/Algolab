#include<iostream>
#include<vector>
#include<algorithm>
#include<climits>
#include<utility>

//from github limo1996/AlgoLab-2017
//BGLincludes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>

using namespace std;
using namespace boost;

struct edge_component_t{
    enum{  num = 555};
    typedef edge_property_tag kind;
} edge_component;

typedef adjacency_list<vecS,vecS, undirectedS, no_property, property<edge_component_t,int>>Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;      //Vertex type
typedef graph_traits<Graph>::edge_descriptor Edge;          //Edgetype
typedef graph_traits<Graph>::edge_iterator EdgeIt;          // Edge iterator
typedef property_map<Graph,edge_component_t>::type ComponentMap;  //Property map edge->weight

int cmp(pair<int,int>p1, pair<int,int>p2){
    if(p1.first ==  p2.first)
        return p1.second < p2.second;
    else
        return p1.first < p2.first;
}
pair<int,int>correctPair(pair<int,int>p){
    pair<int,int> res = p;
    if(p.first > p.second){
        res.first = p.second;
        res.second = p.first;
    }
    return res;
}
void Bridges(){
    //read in
    int islands, bridges;
    cin >> islands >> bridges;

    vector<pair<int,int> > res;

    //create graph
    Graph g(bridges);
    for(int i = 0; i <  bridges; i++){
        int  x0, x1;
        cin >> x0 >> x1;
        add_edge(x0, x1, g);
    }

    //find biconnected components
    ComponentMap components  = get(edge_component,g);
    int nc = biconnected_components(g, components);
    vector<vector<pair<int,int> > > edges(nc);
    EdgeIt it, itEnd;

    //add edges that belong to certain component, idea: edges that connect two component are a single component
    //with only one edge
    for(tie(it,itEnd) = boost::edges(g); it!=itEnd; ++it){
        pair<int,int> p;
        p.first = source(*it,g);
        p.second = target(*it,g);
        edges[components[*it]].push_back(p);
    }

    //check for components with only one edge
    for(int i  = 0;  i< nc; i++){
        if(edges[i].size() == 1){
            res.push_back(correctPair(edges[i][0]));
        }
    }

    //sort
    sort(res.begin(), res.end(), cmp);
    cout << res.size() << endl;
    for(int i  = 0; i < res.size(); i++){
        cout << res[i].first << " " << res[i].second << endl;
    }

}
int main() {
    int n;
    cin >> n;
    while(n--){
        Bridges();
    }
    return 0;
}