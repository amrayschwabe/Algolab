#include <iostream>
#include <cassert>
#include <vector>
#include <climits>
#include <queue>
#include <algorithm>
#include <set>

using namespace std;
void testcase(){
    long nr_critical_points, optimal_trip_length, maximum_risk;
    cin >> nr_critical_points >> optimal_trip_length >> maximum_risk;

    vector<long> temperatures(nr_critical_points);
    for(long i = 0; i < nr_critical_points; ++i){
        cin >> temperatures[i];
       /* if(i == 0 || i == 222 ||i == 15 ||i == 96 ||i == 975){
            cout << "i " << i << " temp " << temperatures[i] << endl;
        }*/
    }

    vector<vector<long> >incident_edges(nr_critical_points, vector<long>(0,0));
    for(long i = 0; i < nr_critical_points-1; ++i){
        long from, to;
        cin >> from >> to;
        //cout << " from " << from << " to " << to << endl;
        incident_edges[from].push_back(to);
    }
    vector<vector<long> >until(nr_critical_points);
    vector<long> max(nr_critical_points);
    vector<long> min(nr_critical_points);
    until[0].push_back(0);
    max[0] = temperatures[0];
    min[0] = temperatures[0];

    set<long> result;

    // BFS to find vertex set S
    std::vector<long> vis(nr_critical_points, false); // visited flags
    std::queue<long> Q; // BFS queue (from std:: not boost::)
    vis[0] = true; // Mark the source as visited
    Q.push(0);
    while (!Q.empty()) {
        const long u = Q.front();
        //cout << "we're at node " << u << endl;
        Q.pop();
        for (long i : incident_edges[u]) {
            if (vis[i]) continue;
            //cout << "looking at child " << i << endl;
            //if inside min/max from before, just add whole vector including this node to next one
            long currtemp = temperatures[i];
            if(currtemp <= max[u] && currtemp >= min[u]){
                /*if(i == 96){
                    cout << "case between limits" << endl;
                }*/
                //cout << "case between limits " << endl;
                //cout << "we used to have the following vector: " << endl;
                /*for(long m : until[u]){
                    cout << m << " ";
                }*/
                int mintemp = temperatures[i];
                int maxtemp = temperatures[i];
                if(until[u].size() >= optimal_trip_length-1){
                    result.insert(until[u][0]);
                    for(int j = 1; j < optimal_trip_length-1; ++j){
                        until[i].push_back(until[u][j]);
                        int temp = temperatures[until[u][j]];
                        mintemp = std::min(mintemp, temp);
                        maxtemp = std::max(maxtemp, temp);
                    }
                    min[i] = mintemp;
                    max[i] = maxtemp;
                }else{
                    until[i] = until[u];
                    min[i] = min[u];
                    max[i] = max[u];
                }
                until[i].push_back(i);
                //cout << "now we have the following vector: " << endl;
                /*for(long m : until[i]){
                    cout << m << " ";
                }*/
            }else if(currtemp > max[u] || currtemp < min[u]){
                //check, maybe it is still okay to keep it
                if(currtemp > max[u] && (currtemp - min[u]) <= maximum_risk){
                    //cout << "case above max but okay " << endl;
                    //just update max and keep everything
                    //cout << "we used to have the following vector: " << endl;
                   /* for(long m : until[u]){
                        cout << m << " ";
                    }*/
                    int mintemp = temperatures[i];
                    int maxtemp = temperatures[i];
                    if(until[u].size() >= optimal_trip_length-1){
                        result.insert(until[u][0]);
                        for(int j = 1; j < optimal_trip_length-1; ++j){
                            until[i].push_back(until[u][j]);
                            int temp = temperatures[until[u][j]];
                            mintemp = std::min(mintemp, temp);
                            maxtemp = std::max(maxtemp, temp);
                        }
                        min[i] = mintemp;
                        max[i] = maxtemp;
                    }else{
                        until[i] = until[u];
                        min[i] = min[u];
                        max[i] = currtemp;
                    }
                    until[i].push_back(i);
                }else if(currtemp < min[u] && (max[u]- currtemp <= maximum_risk)){
                    //cout << "case below min but okay" << endl;
                    //just update min and keep stuff
                    int mintemp = temperatures[i];
                    int maxtemp = temperatures[i];
                    if(until[u].size() >= optimal_trip_length-1){
                        result.insert(until[u][0]);
                        for(int j = 1; j < optimal_trip_length-1; ++j){
                            until[i].push_back(until[u][j]);
                            int temp = temperatures[until[u][j]];
                            mintemp = std::min(mintemp, temp);
                            maxtemp = std::max(maxtemp, temp);
                        }
                        min[i] = mintemp;
                        max[i] = maxtemp;
                    }else{
                        until[i] = until[u];
                        min[i] = currtemp;
                        max[i] = max[u];
                    }
                    until[i].push_back(i);
                }else {

                   // cout << "case new" << endl;
                    //cout << "until now we had the following vector: " << endl;
                   /* for(long m : until[u]){
                        cout << m << " ";
                    }
                    cout << endl;*/
                    //now we need to drop stuff until it's okay again
                    //add them to output
                    long ucount = until[u].size();
                    long icount = ucount;
                    long count = 0;
                    while (icount - 1 >= 0 && abs(temperatures[until[u][icount - 1]] - currtemp) <= maximum_risk) {
                        count++;
                        icount--;
                    }
                    long newmax = currtemp;
                    long newmin = currtemp;
                    for (long j = ucount - count; j < ucount; ++j) {
                        long temp = temperatures[until[u][j]];
                        newmin = std::min(newmin, temp);
                        newmax = std::max(newmax, temp);
                        until[i].push_back(until[u][j]);
                    }
                    until[i].push_back(i);
                    //cout << "ucount " << ucount << " count " << count << endl;
                    min[i] = newmin;
                    max[i] = newmax;
                    //cout << "min got updated to " << newmin << " and max got updated to " << newmax << endl;
                    //cout << "now we have the following vector: " << endl;
                    /*for(long m : until[i]){
                        cout << m << " ";
                    }
                    cout << endl;*//*
                   if(i == 1){
                        cout << "until 1" << endl;
                        for(long m : until[i]){
                            cout << m << " ";
                        }
                        cout << endl;
                    }*/
                }

            }
           // cout << "min got updated to " << min[i] << " and max got updated to " << max[i] << endl;
            vis[i] = true;
            Q.push(i);
        }
        until[u].clear();
    }

    if(result.size() == 0){
        cout << "Abort mission";
    }else{
        //sort(result.begin(), result.end());
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