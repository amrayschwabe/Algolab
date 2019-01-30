#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <climits>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

using namespace std;

// from slides, fun!
double floor_to_double(const K::FT& x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}

double ceil_to_double(const K::FT& x)
{
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a += 1;
    while (a-1 >= x) a -= 1;
    return a;
}

int main(){
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(0);
    while(true){
        // read number of graypes
        long nr;
        std::cin >> nr;
        if(nr < 1){
            return 0;
        }
        // read points
        std::vector<K::Point_2> pts;
        pts.reserve(nr);
        for (std::size_t i = 0; i < nr; ++i) {
            K::Point_2 p;
            std::cin >> p;
            pts.push_back(p);
        }
        // construct triangulation
        Triangulation t;
        t.insert(pts.begin(), pts.end());
        long minlength = INT_MAX;
        for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e){
            long length = t.segment(e).squared_length();
            if(length < minlength){
                minlength = length;
            }
        }
        cout << ceil_to_double((sqrt(minlength))/2*100) << endl;
    }
}
