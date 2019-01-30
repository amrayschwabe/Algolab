#include <iostream>
#include <vector>
#include <set>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangle_2.h>
#include <algorithm>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Triangle_2 T;


using namespace std;
vector< pair<P, P> > sortLines(P a1, P a2, P b1, P b2, P c1, P c2){
    vector< pair<P,P> > result;
    if(orientation(a1,a2,b1) == CGAL::LEFT_TURN){
        pair<P,P> a(a1,a2);
        result.push_back(a);
    } else {
        pair<P,P> a(a2,a1);
        result.push_back(a);
    }
    if(orientation(b1,b2,c1) == CGAL::LEFT_TURN){
        pair<P,P> b(b1,b2);
        result.push_back(b);
    } else {
        pair<P,P> b(b2,b1);
        result.push_back(b);
    }
    if(orientation(c1,c2,a1) == CGAL::LEFT_TURN){
        pair<P,P> c(c1,c2);
        result.push_back(c);
    } else {
        pair<P,P> c(c2,c1);
        result.push_back(c);
    }
    return result;
}
void testcase() {
    long legs, mapparts;
    cin >> legs >> mapparts;
    vector<P> path;
    vector<T> map;
    vector<int> useful;
    vector<set<int>> pint(mapparts);
    int x;
    int y;
    int q1x, q1y, q2x, q2y, q3x, q3y, q4x, q4y, q5x, q5y, q6x, q6y;
    vector<set<int>> positions(mapparts);
    vector<int> alreadyin(legs, 0);
    int start = 0, end = mapparts, beststart = 0, bestend = mapparts;
    int count = 0;

    for (int i = 0; i < legs; i++) {
        cin >> x >> y;
        path.push_back(P(x, y));
    }
    for (int j = 0; j < mapparts; j++) {
        cin >> q1x >> q1y >> q2x >> q2y >> q3x >> q3y >> q4x >> q4y >> q5x >> q5y >> q6x >> q6y;
        vector<pair<P, P> > oriented_lines = sortLines(P(q1x, q1y), P(q2x, q2y), P(q3x, q3y), P(q4x, q4y), P(q5x, q5y),
                                                       P(q6x, q6y));
        //cout << "loop j start" << endl;
        vector<int> inTriangle(legs, 1);
        for (int i = 0; i < legs; ++i) {
            P a = path[i];
            //cout << "loop i start" << endl;
            //cout << "i " << i << endl;
            for (vector<pair<P, P>>::iterator it = oriented_lines.begin(); it != oriented_lines.end(); ++it) {
                CGAL::Orientation oriented = orientation(it->first, it->second, a);
                //cout << "loop it start" << endl;
                if (oriented == CGAL::RIGHT_TURN) {
                    inTriangle[i] = 0;
                    //cout << "RIGHT_TURN j" << j << endl;
                }
            }
            if (i > 0) {
                if (inTriangle[i - 1] == 1 && inTriangle[i] == 1) {
                    //cout << "in triangle "  <<j << " " << i << endl;
                    positions[j].insert(i-1);
                    if (alreadyin[i-1] == 0) {
                        alreadyin[i-1] = 1;
                        count++;
                        if (count == legs - 1) {
                            beststart = start;
                            bestend = j;
                        }
                    } else {
                        //cout << "else" << endl;
                        if (start < j) {
                            for (int k = start; k < j; k++) {
                                positions[k].erase(i-1);
                            }
                            while (positions[start].empty() && start < j) {
                                start++;
                            }
                        }
                        if ((j - start < bestend - beststart) && (count == legs - 1)) {
                            bestend = j;
                            beststart = start;
                        }

                    }
                }
            }
        }
        if(count== 0){
            start++;
        }
    }
    cout << (bestend - beststart + 1) << endl;
}

int main() {
    int n;
    cin >> n;
    while(n--){
        testcase();
    }
}