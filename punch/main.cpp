//
// Created by Amray Schwabe on 23.10.18.
//
#include<iostream>
#include<queue>
#include<climits>
#include<algorithm>

using namespace std;

void testcase(){
    int n,k;
    cin >> n >> k;
    vector<pair<int,int> > c_v;
    int max = 0;
    for(int i = 0; i<n; i++){
        int c, v;
        cin >> c >> v;
        c_v.push_back(make_pair(c,v));
        if(v > max){
            max = v;
        }
    }
    max = max + k-1;
    vector<vector<pair<int,int> > >memo(n, vector<pair<int,int>> (max,make_pair(0,0)));
    int cost0 = c_v[0].first;
    int vol0 = c_v[0].second;
    memo[0][0] = make_pair(0,0);
    for(int i = 1; i < max; i++){
        memo[0][i] = make_pair((vol0 + i -1)/vol0 * cost0, 1);
    }
    for(int i = 1; i < n; ++i){
        int cost = c_v[i].first;
        int vol = c_v[i].second;
        for(int j = 1; j < max; ++j){
            int volwo = std::max(0, j-vol);
            int take_cost = memo[i][volwo].first + cost;
            int take_count = memo[i][volwo].second  + (memo[i][volwo] == memo[i-1][volwo] ? 1:0);

            int leave_cost = memo[i-1][j].first;
            int leave_count = memo[i-1][j].second;

            if(take_cost < leave_cost){
                memo[i][j] = make_pair(take_cost, take_count);
            }else if(leave_cost < take_cost){
                memo[i][j] = make_pair(leave_cost, leave_count);
            }else{
                if(take_count >  leave_count){
                    memo[i][j] = make_pair(take_cost, take_count);
                }else{
                    memo[i][j] = make_pair(leave_cost, leave_count);
                }
            }
        }
    }
    cout << memo[n-1][k].first << " " << memo[n-1][k].second << endl;

}



int main() {
    std::ios_base::sync_with_stdio(false);
    int n;
    cin >> n;
    while(n--){
        testcase();
    }
}