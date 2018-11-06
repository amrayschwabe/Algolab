#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>


using namespace std;

int solve(int &wizards, vector<pair<int,int>> &lr){
    int right = INT_MIN;
    int altright = INT_MIN;
    int count =0;
    for(int i = 0; i< wizards; ++i){
        if(right <= lr[i].second){
            altright = right;
            right = max(lr[i].second, right + lr[i].first);
            count ++;
        }else{
            int border = max(altright + lr[i].first, lr[i].second);
            if(border < right){
                right = border;
            }
        }
    }
    return count;
}

void testcase(){
    int wizards;
    cin >> wizards;
    vector<pair<int,int>> lr(wizards);
    for(int i = 0; i < wizards; i++) {
        int length, ring;
        cin >> length>> ring;
        lr[i].first = length;
        lr[i].second = ring;
    }
    sort(lr.begin(), lr.end(), [](pair<int,int> &left, pair<int,int> &right) {
        return left.second< right.second;
    });
    int index = 0;
    int right_border = -1000001;
    int result = solve(wizards, lr);

    cout << result << endl;



}
int main() {
    int n;
    cin >> n;
    while(n--) {
        testcase();
    }
    return 0;
}