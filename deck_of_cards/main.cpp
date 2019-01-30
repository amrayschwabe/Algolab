#include <iostream>
#include <vector>

using namespace std;

void testcasefunction(){
    int k;
    int n;
    cin >> n;
    cin >> k;
    int max_i = 0;
    int max_j = 0;
    int max_k = 1000;
    int i = 0;
    int j = 0;
    int now_k = 0;
    int tmp;
    vector<int> numbers;
    for (int a = 0; a < n; a++) {
        cin >> tmp;
        numbers.push_back(tmp);
        now_k += tmp;
        if (max(k - now_k, now_k - k) < max(max_k-k, k-max_k)) {
            max_k = now_k;
            max_i = i;
            max_j = j;
        }
        if (k - now_k < 0) {
            while (k - now_k < 0 && i < j+1) {
                i++;
                now_k = now_k - numbers[i-1];
                if (max(k - now_k, now_k - k) < max (k -max_k, max_k - k)) {
                    max_k = now_k;
                    max_i = i;
                    max_j = j;
                }
            }
        }
        j++;
    }
    cout << max_i << " " << max_j << endl;
    return;

}





int main() {
    int nr_cases;
    cin >> nr_cases;
    for (int i = 0; i < nr_cases; i++){
        testcasefunction();
    }
    return 0;

}