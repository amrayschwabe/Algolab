#include <iostream>
#include <cassert>
#include <vector>
#include <climits>
#include <algorithm>
#include <set>

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
vector<bool> vis;

bool take_max_over_last_n(int index){
    /*cout << "take max over last n with triplength " << triplength << " and maxrisk " << max_risk << endl;
    cout << " out vector so far is " << endl;
    for(long i : temperatures_on_path){
        cout << i << " ";
    }
    cout << endl;*/
    long maxtemp = temperatures_on_path[index];
    long mintemp = temperatures_on_path[index];
    if(index - triplength + 1 < 0){
        return false;
    }

    for(int i = index; i > index- triplength; --i){
        /*if(index == 207){
            cout << "i " << i << " maxtemp " << maxtemp << " mintemp " << mintemp << endl;
            cout << "temperature on path " << temperatures_on_path[i] << endl;
        }*/
        if(abs(maxtemp- temperatures_on_path[i]) > maxrisk || abs(mintemp- temperatures_on_path[i]) > maxrisk){
            return false;
        }
        maxtemp = max(temperatures_on_path[i], maxtemp);
        mintemp = min(temperatures_on_path[i], mintemp);
    }
    return true;
}
void DFS(int node){
    //cout << "node" << node<< endl;
        //cout << "we're at node " << node << endl;
        /*if(incident_edges[node].size() < 1){
            cout << "no incident edges" << endl;
        }*/
        for (long i : incident_edges[node]) {
            //cout << "looking at node " << i << endl;
            cout << i << endl;
            if (vis[i]) continue;
            int insert_at = where_in_path[parent[i]] + 1;

            temperatures_on_path[insert_at] = temperatures[i];
            where_in_path[i] = insert_at;
            what_in_path[insert_at] = i;
            /*if(i == 876){
                cout << "index " << insert_at << endl;
            }*/
            if(take_max_over_last_n(insert_at)){
                result.insert(what_in_path[insert_at-triplength +1]);
            }
            vis[i] = true;
            //cout << "node" << node<< endl;
            //cout << "recursion with " << i << endl;
            cout << i << endl;
            DFS(i);
        }
    }
void testcase(){

    long nr_critical_points, optimal_trip_length, maximum_risk;
    cin >> nr_critical_points >> optimal_trip_length >> maximum_risk;

    temperatures_on_path = vector<long>(nr_critical_points);
    maxrisk = maximum_risk;
    triplength = optimal_trip_length;
    temperatures = vector<long>(nr_critical_points);
    vector<vector<long> > new_incident_edges(nr_critical_points);
    incident_edges.clear();
    incident_edges = new_incident_edges;
    parent = vector<long>(nr_critical_points);
    what_in_path = vector<long>(nr_critical_points);
    where_in_path = vector<long>(nr_critical_points);
    vis = vector<bool>(nr_critical_points, false);
    result.clear();

    for(long i = 0; i < nr_critical_points; ++i){
        cin >> temperatures[i];
        /*if(i == 360 ||i == 911 || i == 168 || i == 589 ||i == 876){
            cout << "i " << i << " " <<temperatures[i] << endl;
        }*/
    }

    for(long i = 0; i < nr_critical_points-1; ++i){
        long from, to;
        cin >> from >> to;
        incident_edges[from].push_back(to);

        parent[to] = from;
    }

    where_in_path[0] = 0;
    what_in_path[0] = 0;

    temperatures_on_path[0] = temperatures[0];


    // DFS to find vertex set S
    vis[0] = true; // Mark the source as visited
    DFS(0);

    if(result.size() == 0){
        cout << "Abort mission";
    }else{
        for(long i : result){
            cout << i << " ";
        }
    }
    cout << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    long t;
    cin >> t;
    while(t--){
        testcase();
    }
    return 0;
}