#include <iostream>

#include <string>
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <cassert>

#include <boost/graph/max_cardinality_matching.hpp>
using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS> my_graph;

void buddy(){
    int buddies, hobbies, minhob, hobbyId = 0;
    cin >> buddies >> hobbies >> minhob;
    map<string, int> nametoint;
    map<pair<int, int>, int> edgesCount;
    vector<set<int> > perstohobby(buddies);
    for (int i = 0; i<buddies; i++){
        for(int j= 0; j< hobbies; j++){
            string tmp;
            cin >> tmp;
            if(nametoint.find(tmp) == nametoint.end()){
                nametoint[tmp] = hobbyId++;
            }
            perstohobby[i].insert(nametoint[tmp]);
        }
    }
    my_graph connections;
    for(int i = 0; i < buddies; i++){
        for(int j = i; j < buddies; j++){
            vector<int> intersection;
            set_intersection(perstohobby[i].begin(), perstohobby[i].end(),perstohobby[j].begin(),perstohobby[j].end(),back_inserter(intersection));
            if(intersection.size() >  minhob){
                add_edge(i,j,connections);
            }
        }
    }
    vector<graph_traits<my_graph>::vertex_descriptor> mate(buddies);
    edmonds_maximum_cardinality_matching(connections, &mate[0]);
    int matchingsize = matching_size(connections, &mate[0]);

    if(matchingsize < buddies/2){
        cout << "optimal" << endl;
    }
    else{
        cout << "not optimal" << endl;
    }

}

int main(){
    int n;
    cin >> n;
    while(n--){
        buddy();
    }
    return 0;
}