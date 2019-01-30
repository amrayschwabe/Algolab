#include <vector>
#include <iostream>
#include <bitset>
#include <climits>
#include <algorithm>

using namespace std;

struct team{
	int nr_free_players;
	int difference;
};

bool cmp(team a, team b){
	return ((a.difference < b.difference) ||((a.difference == b.difference) && (a.nr_free_players < b.nr_free_players)));
}

bool cmp_search(team a, team b){
	return (a.difference < b.difference);
}

bool cmp_search_smaller(team a, team b){
	return a.nr_free_players < b.nr_free_players;
}

vector<team> solve(vector<int>&skill_level, int upper_b){
	int nr_players = skill_level.size();
	vector<team> possible_teams;
	for(int i = 0; i < (1 << nr_players); ++i){
		int strength1 = 0;
		int players1 = 0;
		for(int k = 0; k < nr_players; ++k){
			if(i & 1 << k){
				strength1 += skill_level[k];
				players1 ++;
			}
		}
		for(int j = 0; j < (1<<nr_players); ++j){
			int strength2 = 0;
			int players2 = 0;
			bool impossible = false;
			for(int k = 0; k < nr_players; ++k){
				if(j& 1 << k && i&1 <<k){
					impossible = true;
					break;
				}
				if((j & 1 <<k) && !(i & 1 << k)){
					strength2 += skill_level[k];
					players2 ++;
				}
			}
			int free_players = nr_players-players1 -players2;
			int difference = strength1 - strength2;
			if(free_players <= upper_b && !impossible) {
				team t = {free_players, difference};
				possible_teams.push_back(t);
			}
		}

	}
	return possible_teams;
}
void testcase() {
	int nr_students, upper_b;
	cin >> nr_students >> upper_b;

	int first_half = nr_students /2;
	int second_half = nr_students - first_half;

	vector<int>skill_level_fh(first_half);
	vector<int>skill_level_sh(second_half);
	for(int i = 0; i < first_half; ++i) {
		cin >> skill_level_fh[i];
	}
	for(int i = 0; i < second_half; ++i){
		cin >> skill_level_sh[i];
	}

	vector<team> result_first_half = solve(skill_level_fh, upper_b);
	vector<team> result_second_half = solve(skill_level_sh, upper_b);

	sort(result_second_half.begin(),result_second_half.end(), cmp);

	long long result = 0;
	for(int i = 0; i < result_first_half.size(); ++i){
		int nr_free_players = result_first_half[i].nr_free_players;
		//cout << "nr_free_players " << nr_free_players << endl;
		int difference = result_first_half[i].difference;
		team resultteam = {(upper_b-nr_free_players), -difference};
		auto lowerbound = lower_bound(result_second_half.begin(),result_second_half.end(), resultteam,cmp_search);
		auto upperbound = upper_bound(result_second_half.begin(),result_second_half.end(), resultteam,cmp_search);
		auto playerbound = upper_bound(lowerbound, upperbound, resultteam, cmp_search_smaller);
		//cout << "difference " << difference << " l " << lowerbound - result_second_half.begin()<< " u " << upperbound - result_second_half.begin()<<
			 //" p " << playerbound - result_second_half.begin() << endl;
		result += (playerbound-lowerbound);
	}
	cout << result << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t; cin >> t;
	while(t--)
		testcase();
}