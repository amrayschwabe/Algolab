// example: find the risk-optimal investment strategy
// in Swatch and Credit Suisse shares (data as in lecture)
#include <iostream>
#include <cassert>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
using namespace std;

// choose exact rational type
#include <CGAL/Gmpq.h>
typedef CGAL::Gmpq ET;
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



int main() {




    cout << fixed << setprecision(0);
    int nr_nutritiens, nr_foods;
    while(true) {
        Program qp(CGAL::SMALLER, true, 0, false, 0);
        // by default, we have a nonnegative QP with Ax <= b

        cin >> nr_nutritiens >> nr_foods;
        //cout << "numbers " << nr_nutritiens << " "  << nr_foods << endl;
        if ((nr_nutritiens == 0) && (nr_foods == 0)) {
            return 0;
        }
        //read in nutritients
        vector <pair<int, int>> nutri(nr_nutritiens);
        for (int i = 0; i < nr_nutritiens; ++i) {
            cin >> nutri[i].first;
            cin >> nutri[i].second;
            //cout << "nutris " <<nutri[i].first <<" " << nutri[i].second << endl;
        }
        //read in food items
        //cout <<  "alive" << endl;
        vector<int> foodprice(nr_foods);
        vector <vector<int>> nutrilist(nr_foods, vector<int>(nr_nutritiens));
        for (int i = 0; i < nr_foods; ++i) {
            //cin >> foodprice[i];
            //cout << "foodprice" << foodprice[i] << endl;
            int price;
            cin >> price;
            qp.set_c(i, price);
            for (int j = 0; j < nr_nutritiens; ++j) {
                cin >> nutrilist[i][j];
            }
        }
        for (int j = 0; j < nr_nutritiens; ++j) {
            for (int i = 0; i < nr_foods; ++i) {
                qp.set_a(i, j, nutrilist[i][j]);
                //cout << "nutrilist[i][j] " <<  nutrilist[i][j] << endl;
                qp.set_a(i, j + nr_nutritiens, nutrilist[i][j]);
            }
        }
        for (int i = 0; i < nr_nutritiens; ++i) {
            qp.set_b(i, nutri[i].first);
            qp.set_r(i, CGAL::LARGER);
            qp.set_b(i + nr_nutritiens, nutri[i].second);

        }/*
        int counter = 0;
        for(int j = 0; j< nr_nutritiens; ++j){
            for(int i = 0; i < nr_foods; ++i){
                qp.set_a(i, counter, nutrilist[i][j]);
            }
            qp.set_b(counter, nutri[j].second);
            counter++;

        }
        for(int j = 0; j< nr_nutritiens; ++j){
            for(int i = 0; i < nr_foods; ++i){
                qp.set_a(i, counter, nutrilist[i][j]);
            }
            qp.set_b(counter, nutri[j].first);
            qp.set_r(counter, CGAL::LARGER);
            counter++;

        }*/

        // solve the program, using ET as the exact type
        Solution s = CGAL::solve_nonnegative_linear_program(qp, ET());
        assert(s.solves_quadratic_program(qp));

        // output
        if (!s.is_optimal()) {
            std::cout << "No such diet." << endl;
        } else {
            assert(s.status() == CGAL::QP_OPTIMAL);
            std::cout << round_down_to_double(s.objective_value()) << endl;
        }
    }

}
