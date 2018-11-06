
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int subproblem(const int &from, const int &to, const vector<int> &v, const int &k, const int &m, vector<vector<int>> &memo, vector<int> &choices) {

    if (m == 0) {
        return 0;
    }

    if (from >= to) {
        return -1000000;
    }

    //// Compute taking from
    //int sum = 0;
    //int i = from;
    //while (i < v.size() && k > sum) {
    //	sum += v[i];
    //	i++;
    //}

    // it is possible to take it!
    if (choices[from] != -1) {
        int i = choices[from] + 1;
        if (memo[from + 1][m] == -1) {
            memo[from + 1][m] = subproblem(from + 1, to, v, k, m, memo, choices);

        }
        if (memo[i][m - 1] == -1) {
            memo[i][m - 1] = subproblem(i, to, v, k, m - 1, memo, choices);

        }
        return max(memo[from + 1][m], (i - from) + memo[i][m - 1]);
        //return max(subproblem(from + 1, to, v, k, m, memo), (i - from) + subproblem(i, to, v, k, m - 1, memo));
    }
    else {
        if (memo[from + 1][m] == -1) {
            memo[from + 1][m] = subproblem(from + 1, to, v, k, m, memo, choices);
        }
        return memo[from + 1][m];
        //return subproblem(from + 1, to, v, k, m, memo);
    }

}

void testcase() {
    int n, m, k;
    cin >> n; cin >> m; cin >> k;
    vector<int> v(n);
    vector<vector<int>> memo(n + 1, vector<int>(m + 1, -1));
    for (int i = 0; i < n; i++) {
        cin >> v[i];
    }

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