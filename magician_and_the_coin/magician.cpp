#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

//void probfunction(int m, int k, int n) {
//	vector<double> probabilities(m + 1, 0);
//	probabilities[k] = 1;
//	for (int i = 0; i < n; ++i) { // days
//		for (int j = 0; j < m; j++) { // target
//			if (probabilities[j] > 0) {
//				for (int l = 1; l <= j; l++) { // bets
//					if (probabilities[j] * p[i] > (j + l > m ? probabilities[m] : probabilities[j + l])) {
//						if (j + l < m)
//							probabilities[j + l] = probabilities[j] * p[i];
//						else
//							probabilities[m] = probabilities[j] * p[i];
//					}
//				}
//			}
//		}
//	}
//
//	cout << std::fixed << std::setprecision(5);
//	cout << probabilities[m] << endl;
//}

double probFunction(const vector<double> &p, const int &m, const int &n, int i, int k, vector<vector<double>> &mem) {
	if (k >= m) {
		return 1.0;
	}
	if (i >= n) {
		return 0.0;
	}
	if (i == n - 1) {
		int bet = m - k;
		if (bet > k)
			return 0.0;
		else
			return p[i];
	}
	if (k == 0) {
		return 0.0;
	}
	double maxProb = 0;
	for (int bet = 0; bet <= k; bet++) {
		if (k + bet > m) {
			continue;
		}
		double winProb, looseProb;
		if (mem[k + bet][i + 1] != -1) {
			winProb = mem[k + bet][i + 1];
		}
		else {
			winProb = probFunction(p, m, n, i + 1, k + bet, mem);
			mem[k + bet][i + 1] = winProb;
		}
		if (mem[k - bet][i + 1] != -1) {
			looseProb = mem[k - bet][i + 1];
		}
		else {
			looseProb = probFunction(p, m, n, i + 1, k - bet, mem);
			mem[k - bet][i + 1] = looseProb;
		}
		double newProb = p[i] * winProb + (1 - p[i]) * looseProb;
		maxProb = max(maxProb, newProb);
	}
	return maxProb;
}

void testcase() {
	int n, k, m;
	cin >> n; cin >> k; cin >> m;
	vector<double> p;
	for (int i = 0; i < n; i++) {
		double tmp;
		cin >> tmp;
		p.push_back(tmp);
	}

	vector<vector<double>> mem(m + 1, vector<double>(n, -1));

	cout << std::fixed << std::setprecision(5);
	cout << probFunction(p, m, n, 0, k, mem) << endl;
}

int main() {
	int t; cin >> t;
	for (int i = 0; i < t; i++) {
		testcase();
	}
}