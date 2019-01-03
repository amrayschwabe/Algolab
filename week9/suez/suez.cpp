// example: decide whether there exists a disk that covers a given set
// of points R in the plane and is disjoint from another set of points B
#include <iostream>
#include <cassert>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <vector>
#include <climits>
#include <algorithm>

// choose exact integral type
#include <CGAL/Gmpq.h>
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef CGAL::Quotient<ET> SolT;

using namespace std;
// round up to next integer double
double ceil_to_double(const SolT& x)
{
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a += 1;
    while (a-1 >= x) a -= 1;
    return a;
}

struct nail{
    double x;
    double y;
};
void testcase(){

    //read in
    int nr_new, nr_old, height, width;
    cin >> nr_new >> nr_old >> height >> width;

    vector<nail> freenails;
    for(int i = 0; i < nr_new; ++i){
        nail nail1;
        cin >> nail1.x >> nail1.y;
        freenails.push_back(nail1);
    }

    vector<nail> occnails;
    for(int i = 0; i < nr_old; ++i){
        nail nail1;
        cin >> nail1.x >> nail1.y;
        occnails.push_back(nail1);
    }
    // by default, we have an LP with Ax <= b and no bounds for
    // the four variables alpha, beta, gamma, delta
    Program lp (CGAL::SMALLER, true, 1, false, 0);

    int count = 0;

    //add constraints for new ones, shouldn't touch each other
    for(int i = 0; i < nr_new; ++i){
        for(int j = i+1; j < nr_new; ++j){
            //distance width
            double distw = 2*abs(freenails[i].x - freenails[j].x);
            //distance height
            double disth = 2*abs(freenails[i].y - freenails[j].y);
            //cout << disth << " " << distw << endl;
            if(distw *height < disth * width){
                lp.set_a(i, count, height);
                lp.set_a(j, count, height);
                lp.set_b(count, disth);
            }else{
                lp.set_a(i, count, width);
                lp.set_a(j, count, width);
                lp.set_b(count, distw);
            }
            count ++;

        }
    }

    //for new vs old ones, only care about old one that is closest

    int closest;
    for(int i = 0; i < nr_new; ++i){
        double minh = INT_MAX;
        double minw = INT_MAX;
        for(int j = 0; j < nr_old; ++j){
            //distance width
            double distw = 2*abs(freenails[i].x - occnails[j].x)-width;
            //distance height
            double disth = 2*abs(freenails[i].y - occnails[j].y)-height;
            if(distw * height <  disth * width){
                minh = min(minh,disth);
            }else{
                minw = min(minw,distw);
            }
        }
        if(minh == INT_MAX && minw == INT_MAX){
            break;
        }
        if(minw/width < minh/height){
            lp.set_a(i, count, width);
            lp.set_b(count, minw);
        }else if(minh/height < minw/width){
            lp.set_a(i, count, height);
            lp.set_b(count, minh);
        }
        count ++;
    }

    // objective function:
    for(int i = 0; i < nr_new; ++i){
        lp.set_c(i, -2 *(height+width));
    }

    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert (s.solves_linear_program(lp));

   // cout << s << endl;

    cout << setprecision(0) << fixed << (ceil_to_double(-s.objective_value())) << endl;

}

int main() {
    int t; cin >> t;
    while(t--){
        testcase();
    }

    return 0;
}