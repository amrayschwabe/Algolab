#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <limits>
#include <set>

using namespace std;

void testcasefunction(){
    int nr_words;
    cin >> nr_words;
    vector<int> word_occurrences;
    int temp;
    for(int i = 0; i < nr_words; i++){
        cin >> temp;
        word_occurrences.push_back(temp);
    }


	set<pair<int, int>> allWords_set;
	int index = 0;
	for (auto i : word_occurrences) {
		for (int j = 0; j < i; j++) {
			cin >> temp;
			allWords_set.insert(make_pair(temp, index));
		}
		index++;
	}

	vector<pair<int, int>> allWords(allWords_set.begin(), allWords_set.end());
	queue<pair<int,int>> sequence;
	vector<int> seq_occurrences(nr_words, 0);

	auto complete = [&seq_occurrences]() {
		for (auto el : seq_occurrences) {
			if (el < 1) {
				return false;
			}
		}
		return true;
	};
	bool COMPLETE = false;
	int i = 0;
	int j = numeric_limits<int>::max();
	int best_i = 0;
	int best_j = numeric_limits<int>::max();
	int numberWords = 0;
	for (int k = 0; k < allWords.size(); k++) {
		j = k;
		if (!COMPLETE && seq_occurrences[allWords[k].second] == 0) {
			numberWords++;
			if (numberWords == nr_words) {
				COMPLETE = true;
			}
		}

		seq_occurrences[allWords[k].second]++;
		while (seq_occurrences[allWords[i].second] > 1) {
			seq_occurrences[allWords[i].second]--;
			i++;
		}

		if (COMPLETE && allWords[j].first - allWords[i].first < best_j - best_i) {
			best_j = allWords[j].first;
			best_i = allWords[i].first;
		}
	}
    cout << (best_j-best_i+1) << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
    int nr_testcases;
    cin >> nr_testcases;
    for(int i = 0; i< nr_testcases; i++){
        testcasefunction();
    }
}