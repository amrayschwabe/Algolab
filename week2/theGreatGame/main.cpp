#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int recfunc(vector<vector<int>> &transitions, int position, int turn, int n, int m, vector<vector<int>> &memo){
    if(position == n){
        return  0;
    }
    else{
        if (turn == 0){
            int min = m;
            for(auto i : transitions[position]){
                if (memo[i][1] == -1)
                    memo[i][1] = recfunc(transitions, i, 1, n, m, memo);
                if (min > 1 + memo[i][1]){
                    min  = 1 + memo[i][1];
                }
            }
            return min;
        }
        else{
            int max = 0;
            for (auto i : transitions[position]){
                if (memo[i][0] == -1)
                    memo[i][0] = recfunc(transitions, i, 0, n, m, memo);
                if(max < 1 + memo[i][0]){
                    max = 1 + memo[i][0];
                }
            }
            return max;
        }
    }

}

void testcasefunction(){
    int n, m, r, b;
    cin >> n; cin >> m; cin >> r; cin >> b;

    vector<vector<int>>transitions(n);
    for (int i = 0; i < m; ++i){
        int from, to;
        cin >> from; cin >> to;
        transitions[from].push_back(to);

    }

    vector<vector<int>> memo(n+1, vector<int>(2, -1));

    int black, red;
    red = recfunc(transitions, r, 0, n, m, memo);
    black = recfunc(transitions, b, 0, n, m, memo);
    if (red == black){
        if (red%2 == 1){
            cout << 0 << endl;
        }
        else {
            cout << 1 << endl;
        }
    }
    else if (red < black){
        cout  << 0 << endl;
    }
    else{
        cout << 1 << endl;
    }
}

int main() {

    int t;
    cin >> t;
    for(int i = 0; i < t; i++){
        testcasefunction();
    }
}