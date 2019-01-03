// example: decide whether there exists a disk that covers a given set
// of points R in the plane and is disjoint from another set of points B
#include <iostream>
#include <cassert>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <vector>
#include <climits>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Finite_faces_iterator  Face_iterator;


using namespace std;


// choose exact integral type
#include <CGAL/Gmpq.h>
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<double> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct particle{
    int x;
    int y;
    int d;
};

void testcase(){

    //read in nr_particles, nr_shots, nr_bounty_hunters
    int nr_particles, nr_shots, nr_bounty_hunters;
    cin >> nr_particles >> nr_shots >> nr_bounty_hunters;
    //read in available energy
    int available_energy; cin >> available_energy;

    //read in positions of particles
    vector<particle> particles(nr_particles);
    for(int i=0; i<nr_particles; ++i){
        particle p;
        cin >> p.x >> p.y >> p.d;
        particles[i] = p;
    }

    //read in shooting points
    vector<Triangulation::Point> shootingpoints(nr_shots);
    for(int i = 0; i < nr_shots; ++i){
        Triangulation::Point s;
        cin >> s;
        shootingpoints[i] = s;
    }

    //read in bounty hunters
    vector<Triangulation::Point> bounty_hunters(nr_bounty_hunters);
    for(int i = 0; i < nr_bounty_hunters; ++i){
        Triangulation::Point p;
        cin >> p;
        bounty_hunters[i] = p;
    }

    //triangulate bounty hunters so we can find closest bounty hunter for every shooting point
    Triangulation t;
    t.insert(bounty_hunters.begin(), bounty_hunters.end());
    //iterate over all shooting points and add all the affected points
    vector<vector<pair<int,int> > > affected(nr_particles, vector<pair<int,int> >(0));
    for(int i = 0; i < nr_shots; ++i){
        int distance = INT_MAX;
        Triangulation::Point p = shootingpoints[i];
        if(nr_bounty_hunters > 0) {
            Triangulation::Point nearest = t.nearest_vertex(p)->point();
            distance = CGAL::squared_distance(p, nearest);
        }
        for(int j = 0; j < nr_particles; ++j){
            int shootdist = CGAL::squared_distance(p, Triangulation::Point(particles[j].x, particles[j].y));
            //cout << "shootdist " << shootdist << " distance " << distance << endl;
            if(shootdist< distance){
               // cout << "added shoot i " << i << " to " << j << endl;
               affected[j].push_back(make_pair(i, shootdist));
            }
        }
    }


    //iterate over all particles and add linear equation
    Program lp (CGAL::LARGER, true, 0, false, 0);

    for(int i = 0; i < nr_particles; ++i){
        //cout << "new constraint" << endl;
        for(pair<int,int> j : affected[i]){
            if(j.second == 0){
                j.second = 1;
            }
            //cout << j.first << " * " << 1/j.second << " +  ";
            Triangulation::Point p = shootingpoints[j.first];
            Triangulation::Point p2 = Triangulation::Point(particles[i].x, particles[i].y);
            double inv = 1.0/double((j.second));
            //cout << "inv " << inv << endl;
            lp.set_a(j.first, i, inv);
        }
        //cout << " > " << particles[i].d << endl;
        //add constraint that for every particle density needs to be reached
        //cout << i << " " << particles[i].d << endl;
        lp.set_b(i, particles[i].d);
    }

    //add constraint that sum of shots needs to be smaller than energy
   // cout << "last constraint" << endl;
    for(int i = 0; i < nr_shots; ++i){
        //cout << 1 << " * " << i << " + ";
        lp.set_c(i, 1);
    }



    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert (s.solves_linear_program(lp));

    // output exposure center and radius, if they exist
    //cout << to_double(s.objective_value()) << "  " << available_energy << endl;
    if (!s.is_infeasible() && to_double(s.objective_value()) <= available_energy) {
       cout << "y" << endl;
    } else
       cout << "n" << endl;
}

int main() {
    int t; cin >> t;
    while(t--){
        testcase();
    }
}



/*
// for algolab, by taubnert@ethz.ch
#include <climits>
#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
//#include <CGAL/Gmpz.h>
//typedef CGAL::Gmpz ET;

#include <CGAL/MP_Float.h>
typedef CGAL::MP_Float ET;

using namespace std;

typedef long long dist;

// program and solution types
typedef CGAL::Quadratic_program<double> Program;
typedef CGAL::Quadratic_program_solution <ET> Solution;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Delaunay;
typedef Delaunay::Point Point;
typedef Delaunay::Vertex_handle Vhandle;

dist distance(dist x1, dist y1, dist x2, dist y2) {
    return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
}

void testcase() {
    unsigned a, s, b, e;
    cin >> a >> s >> b;
    cin >> e;

    vector <dist> p_x(a), p_y(a), rho(a);
    for (unsigned i = 0; i < a; i++) {
        cin >> p_x[i] >> p_y[i] >> rho[i];
    }
    vector <dist> s_x(s), s_y(s);
    for (unsigned i = 0; i < s; i++) {
        cin >> s_x[i] >> s_y[i];
    }

    vector <Point> b_p(b);
    for (unsigned i = 0; i < b; i++) {
        cin >> b_p[i];
    }

    Delaunay triangulation;
    triangulation.insert(b_p.begin(), b_p.end());

    vector <dist> min_dist2(s);
    vector <vector<unsigned>> p_affected(a);
    // Find affectable for every shooting position
    for (unsigned i = 0; i < s; i++) {
        // Find nearest bounty hunter
        dist dist2 = LLONG_MAX;
        if (b != 0) {
            Point p = Point(s_x[i], s_y[i]);
            Vhandle closest = triangulation.nearest_vertex(p);
            dist2 = CGAL::squared_distance(closest->point(), p);
        }
        for (unsigned j = 0; j < a; j++) {
            dist d = distance(p_x[j], p_y[j], s_x[i], s_y[i]);
            if (d < dist2) {
                p_affected[j].push_back(i);
            }
        }
    }

    Program lp(CGAL::LARGER, true, 0, false, 0);
    for (unsigned i = 0; i < a; i++) {
        for (auto it = p_affected[i].begin(); it != p_affected[i].end(); ++it) {
            unsigned j = *it;
            dist d = distance(p_x[i], p_y[i], s_x[j], s_y[j]);
            if (d <= 1) {
                d = 1;
            }
            double d_inv = 1.0 / ((double) d);
            //cout << "inv " << d_inv << endl;
            lp.set_a(j, i, d_inv);
        }
        lp.set_b(i, rho[i]);
        cout << i << " " << rho[i]<< endl;
    }

    for (unsigned i = 0; i < s; i++) {
        lp.set_c(i, 1);
    }

    Solution sol = CGAL::solve_linear_program(lp, ET());

    cout  << to_double(sol.objective_value()) << " " << e << endl;

    if (!sol.is_infeasible() && to_double(sol.objective_value()) <= e) {
        cout << "y" << endl;
    } else {
        cout << "n" << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) { testcase(); }
    return 0;
}*/
