

// ========
// STL includes
#include <iostream>
#include <cstdlib>
#include <vector>
#include <cassert>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <algorithm>
#include <set>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

// choose exact rational type
#include <CGAL/Gmpq.h>
typedef CGAL::Gmpq ET;
// solution type the solver provides
typedef CGAL::Quotient<ET> SolT;
// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

// round up to next integer double
double floor_to_double(const SolT& x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}


using namespace std;

struct warehouse{
    int x;
    int y;
    int supply;
    int alcohol;
};
struct stadium{
    int x;
    int y;
    int demand;
    int limit;
};
struct contourline{
    int x;
    int y;
    int r;
};
void testcase(){
    cout << setprecision(0) << fixed;

    //read in nr_warehouse, nr_stadium, nr_contour_lines
    int nr_warehouse, nr_stadium, nr_contour_lines;
    cin >> nr_warehouse >> nr_stadium >> nr_contour_lines;


    //read in warehouses
    vector<warehouse> warehouses(nr_warehouse);
    vector<K::Point_2> coords;
    for(int i = 0; i < nr_warehouse; ++i){
        int x, y, supply, alcohol;
        cin >> x >> y >> supply >> alcohol;
        warehouse neww;
        neww.x = x;
        neww.y = y;
        neww.supply = supply;
        neww.alcohol = alcohol;
        warehouses[i] = neww;
        coords.push_back(K::Point_2(x,y));
    }

    //read in stadiums
    vector<stadium> stadiums(nr_stadium);
    for(int i = 0; i<nr_stadium; ++i){
        int x, y, demand, limit;
        cin >> x >> y >> demand >> limit;
        stadium news;
        news.x = x;
        news.y = y;
        news.demand = demand;
        news.limit = limit;
        stadiums[i] = news;
        coords.push_back(K::Point_2(x,y));
    }

    //read in revenues
    vector<vector<int> > revenues(nr_warehouse, vector<int> (nr_stadium));
    for(int i = 0; i < nr_warehouse; ++i){
        for(int j = 0; j < nr_stadium; ++j){
            cin >> revenues[i][j];
        }
    }


    //read in contour lines
    vector<contourline> contourlines(nr_contour_lines);
    vector<K::Point_2> linecoordinates(nr_contour_lines);
    for(int i = 0; i < nr_contour_lines; ++i){
        contourline newc;
        cin >> newc.x >> newc.y >> newc.r;
        contourlines[i] = newc;
        linecoordinates[i] = K::Point_2(newc.x,newc.y);
    }

    //triangulation over warehouses and stadiums
    // construct triangulation
    Triangulation t;
    t.insert(coords.begin(), coords.end());

    //iterate through contour lines and find relevant ones
    vector<contourline> relevant_lines;
    for(int i = 0; i < nr_contour_lines; ++i){
        K::Point_2 nearest = t.nearest_vertex(linecoordinates[i])->point();
        if(CGAL::squared_distance(nearest, linecoordinates[i]) < contourlines[i].r * contourlines[i].r){
            relevant_lines.push_back(contourlines[i]);
        }
    }

    int relevantlength = relevant_lines.size();
    //for every warehouse save in how many contour lines they are
    vector<set<int> > winside(nr_warehouse);
    for(int i = 0; i < nr_warehouse; ++i){
        for(int j = 0; j < relevantlength; ++j){
            if(CGAL::squared_distance(K::Point_2(warehouses[i].x, warehouses[i].y), K::Point_2(relevant_lines[j].x, relevant_lines[j].y))< relevant_lines[j].r * relevant_lines[j].r){
                winside[i].insert(j);
            }
        }
    }

    //for every stadium save in how many contour lines they are
    vector<set<int> > sinside(nr_stadium);
    for(int i = 0; i < nr_stadium; ++i){
        for(int j = 0; j < relevantlength; ++j){
            if(CGAL::squared_distance(K::Point_2(stadiums[i].x, stadiums[i].y), K::Point_2(relevant_lines[j].x, relevant_lines[j].y))< relevant_lines[j].r * relevant_lines[j].r){
                sinside[i].insert(j);
            }
        }
    }

    for(int i = 0; i < nr_warehouse; ++i){
        for(int j = 0; j < nr_stadium; ++j){
            set<int> returnset;
            set_difference (sinside[j].begin(), sinside[j].end(), winside[i].begin(),winside[i].end(), inserter(returnset, returnset.begin()));
            set<int> returnset2;
            set_difference (winside[i].begin(),winside[i].end(),sinside[j].begin(), sinside[j].end(), inserter(returnset2, returnset2.begin()));
            int tmp = revenues[i][j];
            revenues[i][j] = tmp *100 - returnset.size() - returnset2.size();

        }
    }

    // by default, we have a nonnegative QP with Ax >= b
    Program qp (CGAL::SMALLER, true, 0, false, 0);


    //demands for stadiums and alcohol limit
    for(int i = 0; i < nr_stadium; ++i) {
        for(int j = 0; j < nr_warehouse; ++j){
            qp.set_a(i*nr_warehouse+j,i,1);
            qp.set_a(i*nr_warehouse+j, nr_stadium+i, warehouses[j].alcohol);
        }
        qp.set_b(i, stadiums[i].demand);
        qp.set_r(i, CGAL::EQUAL);
        qp.set_b(nr_stadium+i, stadiums[i].limit*100);
    }

    //supply of warehouses
    for(int i = 0; i < nr_warehouse; ++i){
        for(int j = 0; j < nr_stadium; ++j){
            qp.set_a(j*nr_warehouse+i, 2*nr_stadium+i, 1);
        }
        qp.set_b(2*nr_stadium+i, warehouses[i].supply);
    }

    //maximize revenues
    for(int i = 0; i < nr_stadium; ++i){
        for(int j = 0; j < nr_warehouse; ++j){
            qp.set_c(i*nr_warehouse+j, -revenues[j][i]);
        }
    }

    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(qp, ET());


    // output
    if (s.status() == CGAL::QP_INFEASIBLE) {
        std::cout << "RIOT!" << endl;
    } else {
        assert (s.status() == CGAL::QP_OPTIMAL);
       cout << floor_to_double(-s.objective_value()/ET(100)) << endl;
    }

}

int main() {
    int t;
    cin >> t;
    while(t--){
        testcase();
    }
}
