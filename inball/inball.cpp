// example: decide whether there exists a disk that covers a given set
// of points R in the plane and is disjoint from another set of points B
#include <iostream>
#include <cassert>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

// choose exact integral type
#include <CGAL/Gmpz.h>
typedef CGAL::Gmpz ET;

typedef CGAL::Quotient<ET> SolT;
// program and solution types
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;


double floor_to_double(const SolT& x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}
int main() {
    int nr_inequalities, nr_dimensions;
    cin >> nr_inequalities >> nr_dimensions;
    while(nr_inequalities != 0){
        // by default, we have an LP with Ax <= b and no bounds for
        // the four variables alpha, beta, gamma, delta
        Program lp (CGAL::SMALLER, false, 0, false, 0);
        for(int i = 0; i < nr_inequalities; ++i){
            long norm = 0;
            for(int j = 0; j < nr_dimensions; ++j){
                int a; cin >> a;
                lp.set_a(j, i, a);
                norm += a*a;
            }
            norm = sqrt(norm);
            lp.set_a(nr_dimensions, i, norm);
            int b; cin >> b;
            lp.set_b(i, b);
        }

        // objective function: -delta (the solver minimizes)
       lp.set_c(nr_dimensions, -1);


        // solve the program, using ET as the exact type
        Solution s = CGAL::solve_linear_program(lp, ET());
        //assert (s.solves_linear_program(lp));
        if(s.is_infeasible() || floor_to_double(-s.objective_value()) < 0){
            cout << "none" << endl;
        }else if(s.is_unbounded()){
            cout << "inf" << endl;
        }else{
            cout << floor_to_double(-s.objective_value()) << endl;
        }
        cin >> nr_inequalities >> nr_dimensions;
    }
    return 0;
}
