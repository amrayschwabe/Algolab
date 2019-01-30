
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int subproblem(const int &from, const int &to, const vector<int> &v, const int &k, const int &m, vector<vector<int>> &memo, vector<int> &choices) {

    //if 0 attackers, return 0
    if (m == 0) {
        return 0;
    }

    //if not a valid interval, return really small number
    if (from >= to) {
        return -1000000;
    }


    // it is possible to take it!
    if (choices[from] != -1) {
        int i = choices[from] + 1;
        //do not take this choice
        if (memo[from + 1][m] == -1) {
            memo[from + 1][m] = subproblem(from + 1, to, v, k, m, memo, choices);

        }
        //do take this choice, then continue with the next possible choice
        if (memo[i][m - 1] == -1) {
            memo[i][m - 1] = subproblem(i, to, v, k, m - 1, memo, choices);

        }
        //take max of those two
        return max(memo[from + 1][m], (i - from) + memo[i][m - 1]);
    }
    //not possible to take it, just continue with next field
    else {
        if (memo[from + 1][m] == -1) {
            memo[from + 1][m] = subproblem(from + 1, to, v, k, m, memo, choices);
        }
        return memo[from + 1][m];
    }

}

void testcase() {

    //read in
    int n, m, k;
    cin >> n; cin >> m; cin >> k;
    vector<int> v(n);
    vector<vector<int>> memo(n + 1, vector<int>(m + 1, -1));
    for (int i = 0; i < n; i++) {
        cin >> v[i];
    }

    //at each coordinate, what's the max defense strength you can have starting from there
    vector<int> choices(n, -1);
    int j = 0;
    int sum = v[0];
    for (int i = 0; i < n; i++) {
        if (sum < k) {
            j++;
            if (j >= n) {
                break;
            }
            sum += v[j];
            i--;
        }
        else if (sum == k) {
            choices[i] = j;
            sum -= v[i];
        }
        else { // sum > k
            sum -= v[i];
        }
    }

    //recursivr call
    int maxValue = subproblem(0, n - 1, v, k, m, memo, choices);

    if (maxValue < 0) {
        cout << "fail" << endl;
    }
    else {
        cout << maxValue << endl;
    }
}

int main() {
    int t; cin >> t;
    while (t--) {
        testcase();
    }
}