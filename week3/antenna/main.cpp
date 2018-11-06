#include <iostream>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef  CGAL::Min_circle_2_traits_2<K>  Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;


using namespace std;
double floor_to_double(const K::FT& x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a < x) a += 1;
    while (a-1 >= x) a -= 1;
    return a;
}

void testcasefunction(int n){

    K::Point_2 P[n];
    for(int i = 0; i < n; i++) {
        long x, y;
        cin >> x;
        cin >> y;
        P[i] = K::Point_2(x,y);
    }
    Min_circle mc(P, P+n, true);
    Traits::Circle c = mc.circle();
    cout << fixed << setprecision(0) << floor_to_double(sqrt(c.squared_radius())) << endl;

}

int main() {
    long n; cin >> n;
    while(n){
        testcasefunction(n);
        cin >> n;
    }
    return 0;
}