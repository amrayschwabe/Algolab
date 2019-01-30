// example: find the risk-optimal investment strategy
// in Swatch and Credit Suisse shares (data as in lecture)
#include <iostream>
#include <cassert>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
using namespace std;

// choose exact rational type
#include <CGAL/Gmpz.h>
typedef CGAL::Gmpz ET;
// solution type the solver provides
typedef CGAL::Quotient<ET> SolT;
// program and solution types
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


// round down to next integer double
double round_down_to_double(const SolT& x)
{
    double a = std::ceil(CGAL::to_double(x));
    while (a > x) a -= 1;
    while(a+1<=x)  a +=1;
    return a;
}
// round up to next integer double
double round_up_to_double(const SolT& x)
{
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a += 1;
    while(a-1>=x)  a -=1;
    return a;
}



int main() {

    cout << fixed << setprecision(0);
    int nr_assets, nr_people;
    while(true) {
        cin >> nr_assets  >> nr_people;
        if(nr_assets == 0 && nr_people == 0){
            return 0;
        }
        vector<int> costvec(nr_assets);
        vector<int> returnvec(nr_assets);
        for(int i = 0; i<nr_assets; ++i){
            cin >>costvec[i];
            cin >>returnvec[i];
        }
        vector<vector<int>> covariancevec(nr_assets, vector<int>(nr_assets));
        for(int i = 0; i<nr_assets; ++i){
            for (int j = 0; j<nr_assets; ++j){
                cin >> covariancevec[i][j];
            }
        }
        for(int peopleid = 0; peopleid < nr_people; ++peopleid){

            int max_cost, min_return, max_variance;
            cin >> max_cost >> min_return >> max_variance;
            Program qp(CGAL::SMALLER, true, 0, false, 0);

            //ensure not too expensive
            for(int i = 0; i<nr_assets; ++i){
                qp.set_a(i, 0, costvec[i]);
            }
            qp.set_b(0, max_cost);

            //ensure return big enough
            for(int i = 0; i<nr_assets; ++i){
                qp.set_a(i, 1, returnvec[i]);
            }
            qp.set_b(1, min_return);
            qp.set_r(1, CGAL::LARGER);

            //ensure variance not too big
            for(int i = 0; i<nr_assets; ++i){
                for (int j = i; j<nr_assets; ++j){
                    qp.set_d(j, i, 2*covariancevec[j][i]);
                }
            }
            // solve the program, using ET as the exact type
            Solution s = CGAL::solve_nonnegative_quadratic_program(qp, ET());
            // output
            if (s.status() == CGAL::QP_INFEASIBLE || round_up_to_double(s.objective_value()) > max_variance) {
                std::cout << "No." << endl;
            } else {
                cout << "Yes." << endl;
            }
        }
    }

}

