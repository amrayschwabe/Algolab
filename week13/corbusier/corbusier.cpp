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

void testcases() {
    //read in
    int nr_disks, height, modulo;
    cin >> nr_disks >> height >> modulo;
    vector<long> disks(nr_disks);
    for(int i = 0; i < nr_disks; ++i){
        int in;
        cin >> in;
        in = in % modulo;
        disks[i] = in;
    }


    //init DP
    vector<vector<bool> >DP(modulo, vector<bool>(nr_disks, false));

    //init first row with only first block
    DP[disks[0]][0] = true;

    //fill up dp until one field in the height row gets set to true, otherwise output false
    for(int i = 1; i < nr_disks; ++i){
        for(int j = 0; j < modulo; ++j){
            //check if already feasible with one less block
            if(DP[j][i-1]){
                DP[j][i] = true;
            }
            //otherwise check if (height + modulo - current block)%modulo feasible with one block less, if yes then yes
            if(DP[(j + modulo - disks[i])%modulo][i-1]){
                DP[j][i] = true;
            }
            if(j == height && DP[j][i]){
                cout << "yes" << endl;
                return;
            }
        }
    }
    cout << "no" << endl;
    return;
}

// Main function to loop over the testcases
int main() {
    std::ios_base::sync_with_stdio(false);
    int T;	cin >> T;
    for (; T > 0; --T)	testcases();
    return 0;
}