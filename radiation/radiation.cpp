// example: decide whether there exists a disk that covers a given set
// of points R in the plane and is disjoint from another set of points B
#include <iostream>
#include <cassert>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <vector>

// choose exact integral type
#include <CGAL/Gmpzf.h>
typedef CGAL::Gmpzf ET;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;
struct cell{
    double x;
    double y;
    double z;
};


bool solvable(vector<cell> &healthycells, vector<cell> &cancercells, int dim){
    // by default, we have an LP with Ax <= b and no bounds for
    // the four variables alpha, beta, gamma, delta

    Program qp (CGAL::SMALLER, false, 0, false, 0);
    int nr_cancercells = cancercells.size();
    int nr_healthycells = healthycells.size();

    for(int i = 0; i < nr_cancercells; ++i){
        int counter = 0;
        for(int jx = 0; jx <= dim; ++jx){
            for(int jy = 0; jy <= dim-jx; ++jy){
                for(int jz = 0; jz <= dim-jx-jy; ++jz){
                    double val = pow(cancercells[i].x, jx)*pow(cancercells[i].y, jy)* pow(cancercells[i].z, jz);
                    qp.set_a(counter, i, val);
                    counter ++;
                }
            }
        }
        qp.set_b(i, 0.0001);
        qp.set_r(i, CGAL::LARGER);
    }

    for(int i = 0; i < nr_healthycells; ++i){
        int counter = 0;
        for(int jx = 0; jx <= dim; ++jx){
            for(int jy = 0; jy <= dim-jx; ++jy){
                for(int jz = 0; jz <= dim-jx-jy; ++jz){
                    double val = pow(healthycells[i].x, jx)*pow(healthycells[i].y, jy)* pow(healthycells[i].z, jz);
                    qp.set_a(counter, nr_cancercells+i, val);
                    counter ++;
                }
            }
        }
        qp.set_b(nr_cancercells+i, -0.0001);
    }

    // solve the program, using ET as the exact type
    CGAL::Quadratic_program_options options;
    options.set_pricing_strategy(CGAL::QP_BLAND);
    Solution s = CGAL::solve_linear_program(qp, ET(), options);
    return s.is_infeasible();
}
void testcase(){

    int nr_cancercells;
    int nr_healthycells;
    cin >> nr_healthycells >> nr_cancercells;
    //cout << "nr_healthycells " << nr_healthycells << " nr cancercells " << nr_cancercells << endl;

    vector<cell> healthycells;
    for(int i = 0; i < nr_healthycells; ++i){
        cell newcell;
        cin >> newcell.x >> newcell.y >> newcell.z;
        //cout << newcell.x << " " << newcell.y << " " << newcell.z << " " << i << endl;
        healthycells.push_back(newcell);
    }

    vector<cell> cancercells;
    for(int i = 0; i < nr_cancercells; ++i){
        cell newcell;
        cin >> newcell.x >> newcell.y >> newcell.z;
        //cout << newcell.x << " " << newcell.y << " " << newcell.z << " " << i << endl;
        cancercells.push_back(newcell);
    }

    if(nr_healthycells == 0 || nr_cancercells == 0) {
        cout << 0 << endl;
        return;
    }

    int lmin=1,lmax = 2;
    while (solvable(healthycells, cancercells, lmax)) {
        if (lmax > 30) {
            break;
        }
        lmax = min(2*lmax, 30+1);
    }

    while (lmin != lmax) {
        int p = (lmin+lmax)/2;
        if (solvable(healthycells, cancercells, p))
            lmin = p+1;
        else {
            lmax = p;
        }
    }
    int L = lmin;

    if (L > 30) {
        cout << "Impossible!" << endl;
    } else {
        cout << L << endl;
    }
}

int main() {
    int t;
    cin >> t;
    while(t--){
        testcase();
    }
    return 0;
}