#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
using namespace std;


void testcase(){
    int nr_balls; cin >> nr_balls;
    vector<int> v(nr_balls);
    int timer;
    for(int i = 0; i< nr_balls; i++){
        cin >> timer;
        v[i] = timer;
    }
    for(int i = 0; i <= (nr_balls-3)/2;  i++){
        v[2*i+1] = min(v[2*i+1], v[i]-1);
        v[2*i+2] = min(v[2*i+2], v[i]-1);
    }
    sort(v.begin(), v.end());
    bool ok = true;
    for(int i = 0; i < nr_balls; i++){
        ok = ok && v[i] >= i+1;
    }
    if(ok) {
        cout << "yes" << endl;
    }
    else{
        cout <<"no" << endl;
    }
    return;
}
int main() {
    ios_base::sync_with_stdio(false);
    int n;
    cin >> n;
    while(n--){
        testcase();
    }
}