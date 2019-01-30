// example: find the risk-optimal investment strategy
// in Swatch and Credit Suisse shares (data as in lecture)
#include <iostream>
#include <cassert>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
using namespace std;

// choose exact rational type
#include <CGAL/Gmpq.h>
typedef CGAL::Gmpq ET;
// solution type the solver provides
typedef CGAL::Quotient<ET> SolT;
// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

// round up to next integer double
double round_up_to_double(const SolT& x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}
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
    int p, a, b;
    while(true) {
        cin >> p;
        if(p==0){
            return 0;
        }
        //read in remaining values
        cin >> a >> b;
        if(p==1){

            // by default, we have a nonnegative QP with Ax <= b
            Program qp(CGAL::SMALLER, true, 0, false, 0);

            // now set the non-default entries:
            const int x = 0;
            const int y = 1;

            // constraint on expected return: x+y<=4
            qp.set_a(x, 0, 1);
            qp.set_a(y, 0, 1);
            qp.set_b(0, 4);

            // strategy constraint: 4x + 2y <= ab
            qp.set_a(x, 1, 4);
            qp.set_a(y, 1, 2);
            qp.set_b(1, a*b);

            // strategy constraint: -x + y <= 1
            qp.set_a(x, 2, -1);
            qp.set_a(y, 2, 1);
            qp.set_b(2, 1);

            // objective function: a*x^2-by
            // we need to specify the entries of 2D, on and below the diagonal
            qp.set_d(x, x,2*a); //  a*x^2
            qp.set_c(y, -b); // b*y


            // solve the program, using ET as the exact type
            Solution s = CGAL::solve_nonnegative_quadratic_program(qp, ET());
            assert(s.solves_quadratic_program(qp));

            // output
            if (s.status() == CGAL::QP_INFEASIBLE) {
                std::cout << "no" << endl;
            } else {
                assert(s.status() == CGAL::QP_OPTIMAL);
                Solution::Variable_value_iterator opt = s.variable_values_begin();/*
                ET sw_ratio = opt->numerator() / opt->denominator();
                ET cs_ratio = (opt + 1)->numerator() / (opt + 1)->denominator();
                ET risk = s.objective_value().numerator() / s.objective_value().denominator();
                double sw_percent = ceil_to_double(100 * *opt);*/
                CGAL::Quotient<CGAL::Gmpq> obj_value = s.objective_value() * -1;

                std::cout << round_down_to_double(obj_value) << endl;
            }

        }else{
            // by default, we have a nonnegative QP with Ax <= b
            Program qp(CGAL::LARGER, true, 0, false, 0);

            // now set the non-default entries:
            const int x = 0;
            const int y = 1;
            const int z = 2;

            //x <= 0
            qp.set_u(x, true, 0);

            //y <= 0
            qp.set_u(y, true, 0);
            qp.set_l(x,false);
            qp.set_l(y,false);
            qp.set_l(z,true, 0);

            // constraint on expected return: x+y>=-4
            qp.set_a(x, 0, 1);
            qp.set_a(y, 0, 1);
            qp.set_b(0, -4);

            // strategy constraint: 4x + 2y + z^2 >= -ab, set z^2 = z
            qp.set_a(x, 1, 4);
            qp.set_a(y, 1, 2);
            qp.set_a(z, 1, 1);
            qp.set_b(1, -(a*b));

            // strategy constraint: -x + y >= -1
            qp.set_a(x, 2, -1);
            qp.set_a(y, 2, 1);
            qp.set_b(2, -1);

            // objective function: a*x^2+by+z^4
            // we need to specify the entries of 2D, on and below the diagonal
            qp.set_d(x, x,2*a); //  a*x^2
            qp.set_d(z,z,2);
            qp.set_c(y, b); // b*y


            // solve the program, using ET as the exact type
            Solution s = CGAL::solve_quadratic_program(qp, ET());
            assert(s.solves_quadratic_program(qp));

            // output
            if (s.status() == CGAL::QP_INFEASIBLE) {
                std::cout << "no" << endl;
            } else {
                assert(s.status() == CGAL::QP_OPTIMAL);
                Solution::Variable_value_iterator opt = s.variable_values_begin();/*
                ET sw_ratio = opt->numerator() / opt->denominator();
                ET cs_ratio = (opt + 1)->numerator() / (opt + 1)->denominator();
                ET risk = s.objective_value().numerator() / s.objective_value().denominator();
                double sw_percent = ceil_to_double(100 * *opt);*/
                std::cout << round_up_to_double(s.objective_value())  << endl;
            }

        }

    }
    return 0;
}
