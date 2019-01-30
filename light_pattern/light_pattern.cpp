
#include <iostream>
#include <cassert>
#include <bitset>
#include <vector>
using namespace std;
void testcase(){
    int nr_lights, size_pattern, pattern;
    cin >> nr_lights >> size_pattern >> pattern;

    bitset<32> patternset(pattern);

    //read in initial state
    vector<int>initial_state(nr_lights);
    for(int i = 0; i < nr_lights; ++i){
        cin >> initial_state[i];
    }

    //calculate number of wrong lights per pattern
    //cout << "nr_lights/size pattern " << int(nr_lights/size_pattern) << endl;
    vector<int>wrong_lights(int(nr_lights/size_pattern),0);
    for(int i = 0; i < nr_lights; ++i){
        if(patternset[size_pattern-1-i%size_pattern] != initial_state[i]){
            wrong_lights[int(i/size_pattern)] += 1;
            //cout << "wrong lights " << wrong_lights[int(i/size_pattern)] << " at i " << int(i/size_pattern) << endl;
        }

    }

    int evenswaps = 0;
    int oddswaps = 1;
    int e=0;
    int o = 0;


    //iterate through all patterns and check whether swap or not swap
    for(int i = int(nr_lights/size_pattern)-1; i >= 0; --i){
        //cout << "evenswaps " << evenswaps << " oddswaps " << oddswaps << endl;
        e = wrong_lights[i] + min(evenswaps, oddswaps+1);
        o = size_pattern- wrong_lights[i] + min(evenswaps+1, oddswaps);
        evenswaps = e;
        oddswaps = o;
    }
    cout << min(evenswaps, oddswaps) << endl;
}

int main() {
    int t; cin >> t;
    while(t--){
        testcase();
    }
}

