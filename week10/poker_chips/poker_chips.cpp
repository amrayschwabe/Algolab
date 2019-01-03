#include <vector>
#include <iostream>
#include <algorithm>
#include <bitset>
#include <cassert>
#include <cmath>
#include <map>

using namespace std;

typedef vector<vector<vector<vector<vector<int>>>>> Memo;

int recfunc(vector<int> count, const vector<vector<int>> &stacks, int value, Memo &memo) {
	if (memo[count[0]][count[1]][count[2]][count[3]][count[4]] != -1) {
		return memo[count[0]][count[1]][count[2]][count[3]][count[4]] + value;
	}
		
	// cout << "**count is: ";
	// for (auto c : count) cout << c << " ";
	// cout << "** :-)" << endl;
	
	int nr_stacks = stacks.size();
	
	// sanity check
	for (auto c : count) assert(c >= -1);
	
	int n = 0;
	for (auto c : count) {
		if (c != -1) n++;
	}
	
	// Exit condition
	if (n < 2) {
		return value;
	}
	
	// stores positions that are on top of the stack
	vector<int> top(nr_stacks, -1);
	// stores the how often the color occurs
	vector<int> number(nr_stacks, 0);
	
	// Get top values
	for (int i = 0; i < nr_stacks; ++i) {
		top[i] = (count[i] == 0) ? -1 : stacks[i][count[i]-1];
	}
	
	// Get number values
	for (int i = 0; i < nr_stacks; ++i) {
		for (int j = 0; j < nr_stacks; ++j) {
			if ((top[i] == top[j]) && (top[j] != -1)) {
				number[i]++;
			}
		}
	}
	
	// Best achievable value. lower bound is 0
	int best = 0;
	
	// Go over all subsets. Start at 1, so that the set size is > 0
	for (int s = 1; s < 1 << nr_stacks; ++s) {
		
		// Holds indices of subset
		vector<int> indices;
		bool same = true;
		bool first = true;
		int last;
		for (int i = 0; i < nr_stacks; ++i) {
			if (s & 1<<i) {
				if (first) {
					last = stacks[i][count[i]-1];
					first = false;
					indices.push_back(i);
				} else if (stacks[i][count[i] - 1] != last) {
					same = false;
					break;
				} else {
					indices.push_back(i);
				}
			}
		}
		
		// Ensure that we remove the maximal number of this color
		// as other choice never makes sense
		// Note: if a stack is empty its number entry is 0, so it never gets
		//       chosen as the set size is always > 0
		// Note: all entries of indices of the same color are the same anyway
		if (same && number[indices[0]] != 0) {
			vector<int> reccount = count;
			for (int i = 0; i < indices.size(); ++i) {
				reccount[indices[i]]--;
			}
			
			// Update score value
			int newValue = 0;
			if (indices.size() > 1) {
				newValue += pow(2, (indices.size() - 2));
			}
			
			best = max(best, recfunc(reccount, stacks, newValue, memo));
		}
	}
	memo[count[0]][count[1]][count[2]][count[3]][count[4]] = best;
	return best + value;
}

void testcase() {
	int nr_stacks;
	cin >> nr_stacks;
	vector<vector<int>> stacks(nr_stacks);
	vector<int> count(5, 0);
	for (int i = 0; i < nr_stacks; ++i) {
		int m; cin >> m;
		stacks[i] = vector<int>(m);
		count[i] = m;
	}
	for (int i = 0; i < nr_stacks; ++i) {
		for (int j = 0; j < stacks[i].size(); ++j) {
			int c; cin >> c;
			stacks[i][j] = c;
		}
	}
	
	Memo memo = vector<vector<vector<vector<vector<int>>>>>(count[0] + 1,
				vector<vector<vector<vector<int>>>>(count[1] + 1,
				vector<vector<vector<int>>>(count[2] + 1,
				vector<vector<int>>(count[3] + 1,
				vector<int>(count[4] + 1, -1)))));
	cout << recfunc(count, stacks, 0, memo) << endl;
}

int main() {
	int t; cin >> t;
	while(t--)
		testcase();
}