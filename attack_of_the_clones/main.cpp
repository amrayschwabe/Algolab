#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <map>


using namespace std;

struct Seg {
	int first, second;
	Seg(int s, int e) : first(s), second(e) {};
};
/*
int solve(int &jedis, int&segments, vector<Seg> &nonoverlap, int left, int right){
    int count = 0;
    int right_border = right;
    if (left == 1){
        left = segments+1;
    }

    for(int i = 0; i < nonoverlap.size(); ++i){
        if ((nonoverlap[i].first > right_border) &&(nonoverlap[i].second < left)){
            count ++;
            right_border =  nonoverlap[i].second;
        }

    }
    return count;
}*/

void testcase(){
    int jedis; int segments;
    cin >> jedis >> segments;
    vector<Seg> nonoverlap;
    vector<Seg> overlap;
    vector<Seg> all;
    map<int,int> count;
    for(int i = 0; i < jedis; i++) {
        int left, right;
        cin >> left>> right;
        left--; right--;
        all.push_back(Seg(left,right));
        count[left]++;
        count[right]--;
        if(left > right){
            count[0]++;
        }
    }
    int ol= 0;
    int min = 11;
    int start;
    for(auto i : count){
        ol += i.second;
        if(ol < min){
            min = ol;
            start = i.first;
        }
    }

    for(auto i : all){
        int newstart;
        int newend;
        if(i.first - start < 0){
            newstart = i.first - start + segments;
        }else{
            newstart = i.first - start;
        }
        if(i.second - start < 0){
            newend = i.second - start + segments;
        }
        else{
            newend= i.second - start;
        }
        if(newstart > newend){
            overlap.push_back(Seg(newstart, newend));
        }else{
            nonoverlap.push_back(Seg(newstart, newend));
        }
    }
    auto solve = [&nonoverlap, segments] (int startIndex, int endIndex) {
        int count = 0;
        int right = endIndex;
        if(startIndex == 1){
            startIndex = segments+1;
        }
        for (auto seg : nonoverlap) {
            if (seg.first > right && seg.second < startIndex) {
                count++;
                right = seg.second;
            }
        }
        return count;
    };
    sort(nonoverlap.begin(), nonoverlap.end(), [](Seg &left, Seg &right) {
        return left.second< right.second;
    });

    int result = 0;
    int bestresult = 0;
    int size = overlap.size();
    for(int i = 0; i < size; i++) {
        result = 1 + solve(overlap[i].first, overlap[i].second);
        if(result > bestresult){
            bestresult = result;
        }
    }
    result = solve(1, 1);
    if(result > bestresult){
        bestresult =  result;
    }
    cout << bestresult << endl;



}
int main() {
    ios_base::sync_with_stdio(false);
    int n;
    cin >> n;
    while(n--) {
        testcase();
    }
    return 0;
}
