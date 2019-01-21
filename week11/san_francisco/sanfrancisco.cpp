// example: decide whether there exists a disk that covers a given set
// of polongs R in the plane and is disjolong from another set of polongs B
#include <iostream>
#include <cassert>
#include <vector>
#include <climits>


using namespace std;


void testcase(){
    long nr_holes, nr_canals, score, max_moves;
    cin >> nr_holes >> nr_canals >> score >> max_moves;

    //save all connections with profits
    vector<vector<pair<long, long> > >incident_edges(nr_holes);
    for(long i = 0; i < nr_canals; ++i){
        long from, to, profit;
        cin >> from >> to >> profit;
        incident_edges[from].push_back(make_pair(to, profit));
    }
    //if a hole has no outgoing edges, it must be the one connected to the start
    for(long i = 0; i < nr_holes; ++i){
        if(incident_edges[i].size() < 1){
            incident_edges[i] = incident_edges[0];
        }
    }

    //init DP
    vector<vector<long> >DP(nr_holes, vector<long>(max_moves+1, 0));
    for(long i = 0; i < nr_holes; ++i){
        long max = 0;
        for(auto j : incident_edges[i]){
            if (j.second > max){
                max = j.second;
            }
        }
        DP[i][1] = max;
        //cout << "node nr " << i << " nr moves left " << 1 << " value " << max << endl;
    }

    //iterate through dp
    for(long nr_moves_left = 2; nr_moves_left <= max_moves; nr_moves_left++){
        for(long node_nr = 0; node_nr < nr_holes; ++node_nr){
            long max = 0;
            for(auto j : incident_edges[node_nr]){
                //cout << "node " << j.first << " profit " << j.second << " old value " << DP[j.first][nr_moves_left-1] << endl;
                if ((j.second + DP[j.first][nr_moves_left-1])> max){
                    max = j.second + DP[j.first][nr_moves_left-1];
                }
            }
            if(node_nr == 0 && max >= score){
                cout << nr_moves_left << endl;
                return;
            }
            DP[node_nr][nr_moves_left] = max;
            //cout << "node nr " << node_nr << " nr moves left " << nr_moves_left << " value " << max << endl;
        }
    }
    cout << "Impossible" << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    long t; cin >> t;
    while(t--){
        testcase();
    }
}
