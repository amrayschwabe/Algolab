// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <set>

// Namespaces
using namespace std;



// Functions
// =========
// Function for an individual testcase
void testcases() {
    //read in
    int nr_friends, nr_boxes;
    cin >> nr_friends >> nr_boxes;

    vector<int> friend_strength(nr_friends);
    multiset<int, greater<int> >weight_set;

    //read in friends strength
    int maxstrength = 0;
    for(int i = 0; i < nr_friends; ++i){
        cin >> friend_strength[i];
        maxstrength = max(maxstrength, friend_strength[i]);
    }

    //sort friends strength
    sort(friend_strength.begin(), friend_strength.end(), greater<int>());

    //read in weights
    bool impossible = false;
    for(int i = 0; i < nr_boxes; ++i){
        int weight; cin >> weight;
        if(weight > maxstrength){
            impossible = true;
        }
        weight_set.insert(weight);
    }
    if(impossible){
        cout << "impossible" << endl;
        return;
    }
    //you start at round 0. For every round, iterate over all friends and let them carry the heaviest book they can. If
    //some friend can't carry anything, stop since no other one will be able to carry anything either (since set is sorted)
    int rounds = 0;
    while(!weight_set.empty()){
        rounds ++;
        for(int i = 0; i < nr_friends; ++i){
            int strength_i = friend_strength[i];
            auto bound = weight_set.lower_bound(strength_i);
            if(bound == weight_set.end()){
                break;
            }else{
                weight_set.erase(bound);
            }
        }
    }
    cout << (3*rounds)-1 << endl;
}

// Main function to loop over the testcases
int main() {
    std::ios_base::sync_with_stdio(false);
    int T;	cin >> T;
    for (; T > 0; --T)	testcases();
    return 0;
}