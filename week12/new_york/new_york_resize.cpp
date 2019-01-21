#include <iostream>
#include <cassert>
#include <vector>
#include <climits>
#include <queue>
#include <algorithm>
#include <set>
#include <deque>

using namespace std;
int triplength;
int maxrisk;
vector<long>temperatures_on_path;
vector<long> where_in_path;
vector<long> what_in_path;
set<long> result;
vector<vector<long> > incident_edges;
vector<long> parent;
vector<long> temperatures;
multiset<long>temps;

bool take_max_over_last_n(int index) {
    //cout << "index " << index << endl;
    /*for(int i : temps){
        cout << i << " ";
    }
    cout << endl;*/

    long currtemp = temperatures_on_path[index];
    temps.insert(currtemp);

    if(temps.size() > triplength){
        assert(index-triplength >= 0);
        long temp = temperatures_on_path[index-triplength];
        auto it = temps.find(temp);
        if(it != temps.end()){
            temps.erase(it);
        }
        // assert(triplength == temps.size());
    }

    if(temps.size() == triplength){
        long min = *temps.begin();
        long max = *temps.rbegin();
        if(max - min <= maxrisk){
            return true;
        }
    }
    /*
    if(temps.size() < triplength && index >= triplength-1){
        int tempsize = index-temps.size()+1;
        temps.clear();
        for(int i = index-triplength + 1; i <= index; ++i){
            temps.insert(temperatures_on_path[i]);
        }
        assert(temps.size() == triplength);
    }
*/
    return false;
}
void DFS(int node) {
    for (long i : incident_edges[node]) {
        int insert_at = where_in_path[parent[i]] + 1;
        temperatures_on_path[insert_at] = temperatures[i];
        where_in_path[i] = insert_at;
        what_in_path[insert_at] = i;
        //cout << "at node " << i;
        if (take_max_over_last_n(insert_at)) {
            result.insert(what_in_path[insert_at - triplength + 1]);
        }
        DFS(i);
        /*cout << "temps before erase " << i << endl;
        for(int i : temps){
            cout << i << " ";
        }*/
        auto it = temps.find(temperatures[i]);
        temps.erase(it);
        //cout << "erased " << temperatures[i] << " for index " << i << endl;
        //if necessary, add element triplength away
        if (temps.size() < triplength && temps.size() < insert_at) {
            //cout << "temps. size" << temps.size() << " insert at + 1" << insert_at + 1 << endl;
            long temp = temperatures_on_path[insert_at-triplength];
            //cout << "added " << temp << " for index " << i << endl;
            temps.insert(temp);
        }
        //cout << "temps at end of call " << i << endl;
        /*for(int i : temps){
        cout << i << " ";
        }
        cout << endl;*/

    }
}
void testcase() {

    long nr_critical_points, optimal_trip_length, maximum_risk;
    cin >> nr_critical_points >> optimal_trip_length >> maximum_risk;

    temperatures_on_path.clear();
    temperatures_on_path.resize(nr_critical_points);
    maxrisk = maximum_risk;
    triplength = optimal_trip_length;
    temperatures.clear();
    temperatures.resize(nr_critical_points);
    vector<vector<long> > new_incident_edges(nr_critical_points);
    incident_edges.clear();
    incident_edges = new_incident_edges;
    parent.clear();
    parent.resize(nr_critical_points);
    what_in_path.clear();
    what_in_path.resize(nr_critical_points);
    where_in_path.clear();
    where_in_path.resize(nr_critical_points);
    result.clear();
    temps.clear();

    for (long i = 0; i < nr_critical_points; ++i) {
        cin >> temperatures[i];
    }

    for (long i = 0; i < nr_critical_points - 1; ++i) {
        long from, to;
        cin >> from >> to;
        incident_edges[from].push_back(to);

        parent[to] = from;
    }

    where_in_path[0] = 0;
    what_in_path[0] = 0;
    temps.insert(temperatures[0]);

    temperatures_on_path[0] = temperatures[0];


    DFS(0);

    if (result.size() == 0) {
        cout << "Abort mission";
    }
    else {
        for (long i : result) {
            cout << i << " ";
        }
    }
    cout << endl;

}

int main() {
    ios_base::sync_with_stdio(false);
    long t;
    cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}