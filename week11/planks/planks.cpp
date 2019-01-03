#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <bitset>
#include <cassert>
#include <map>



using namespace std;

int my_sum(bitset<21> tocheck, vector<int>& half){
    int length = half.size();
    int result = 0;
    for(int i = 0; i < length; ++i){
        if(tocheck[i]) {
            result += half[i];
        }
    }
    return result;
}


vector<vector<int> > partition(int side_length, vector<int>& half){
    vector<vector<int> >result;
    int length = half.size();
    bitset<21> ALL = ((1<<length) - 1);
    for(int i = 0; i < 1 <<length; ++i){
        auto set1 = bitset<21>(i);
        for(int j = 0; j < 1 << length; ++j){
            auto set2 = bitset<21>(j);
            bitset<21> AND = set1 & set2;
            bitset<21> INOTJ = set1 & ~set2;
            bitset<21> JNOTI = set2 & ~set1;
            bitset<21> NONE = ~set1 & ~set2;

            //check whether all 4 sets are possible, so all 4 sets are below side length
            int sumAND = my_sum(AND, half);
            int sumINOTJ = my_sum(INOTJ, half);
            int sumJNOTI = my_sum(JNOTI, half);
            int sumNONE = my_sum(NONE, half);
            if(!(max(sumAND, max(sumINOTJ, max(sumJNOTI, sumNONE))) > side_length)){
                vector<int> new_result(4);
                new_result[0] = sumAND;
                new_result[1] = sumINOTJ;
                new_result[2] = sumJNOTI;
                new_result[3] = sumNONE;
                //cout << "sumAND " << sumAND << " sumINOTJ " << sumINOTJ << " sumJNOTI " << sumJNOTI << " sumNONE " << sumNONE << endl;
                result.push_back(new_result);
            }
        }
    }
    //cout << "-----------------------" << endl;
    return result;
}


void testcase(){
    // read in nr planks
    int nr_planks; cin >> nr_planks;

    // read in planks
    vector<int> planks(nr_planks);
    int totalsum = 0;
    for(int i = 0; i < nr_planks; ++i){
        cin >> planks[i];
        totalsum += planks[i];
    }

    //calculate length of each side
    int side_length = totalsum/4;

    //sort planks vector for speedup
    sort(planks.rbegin(), planks.rend());

    //split up in two halfs, split&list for the win
    int nr_half = nr_planks/2;
    int nr_otherhalf = nr_planks -nr_half;
    vector<int> first_half(nr_half);
    vector<int> second_half(nr_otherhalf);
    copy(planks.begin(), planks.begin() + nr_half, first_half.begin());
    copy(planks.begin()+nr_half, planks.end(), second_half.begin());

    //calculate all possible partitions of first and second half of vector
    vector<vector<int> > partitions_first_half = partition(side_length,first_half);
    vector<vector<int> > partitions_second_half = partition(side_length,second_half);

    //sort second half so we can do binary search while iterating over first partitions
    sort(partitions_second_half.begin(), partitions_second_half.end());

    //iterate over first half and check what 4-tuple is missing, then look for that 4-tuple in second half of the partitions
    //if there are multiple 4 tuples, this gives us as many possibilies as there are 4-tuples
    long options = 0;
    int first_partitions_length = partitions_first_half.size();
    for(int i = 0; i < first_partitions_length; ++i){
        vector<int> fourtuple = partitions_first_half[i];
        vector<int> missing(4);
        //cout << "missing ";
        for(int j = 0; j <4; ++j){
            //cout << side_length - fourtuple[j] << " ";
            missing[j] = side_length - fourtuple[j];
        }
        //cout << endl;
        vector<vector<int> >::iterator upperbound, lowerbound;
        upperbound = upper_bound(partitions_second_half.begin(), partitions_second_half.end(), missing);
        lowerbound = lower_bound(partitions_second_half.begin(), partitions_second_half.end(), missing);
        //tie(upperbound, lowerbound) = equal_range(partitions_second_half.begin(), partitions_second_half.end(), missing);
        options += (upperbound - lowerbound);
    }

    //divide result by 24 because we have 4! possible permutations of edges and with 4 tuples we count all of them since order is relevant
    options = options/24;

    //print result
    cout << options << endl;
}

int main() {
    int t; cin >> t;
    while(t--){
        testcase();
    }
}

