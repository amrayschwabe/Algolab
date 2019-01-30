#include <iostream>


#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
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

void testcasefunction(long n){

    K::Point_2 P[n];
    K::Point_2 P2[n];
    for(long i = 0; i < n; i++) {
        long x, y;
        cin >> x;
        cin >> y;
        P[i] = K::Point_2(x,y);
        P2[i] = K::Point_2(x,y);
    }

    if(n <= 2){
        cout << 0 << endl;
        return;
    }
    K::FT rad;
    K::FT minrad;
    Min_circle mc(P2, P2+n, true);
    Traits::Circle c = mc.circle();
    minrad = c.squared_radius();
    for(int j = 0; j < n; j++){
        for(int i = 0; i < mc.number_of_support_points(); i++){
            if(P[j] == mc.support_point(i)){
                P2[j] = P[(j+1)%n];
                Min_circle mc(P2, P2+n, true);
                Traits::Circle c = mc.circle();
                rad = c.squared_radius();
                if(rad < minrad){
                    minrad = rad;
                }
                P2[j] = P[j];
            }
        }
    }

/*
    K::FT rad;
    P2[0] = P2[1];
    Min_circle mc(P2, P2+n, true);
    Traits::Circle c = mc.circle();
    K::FT minrad = c.squared_radius();
    P2[0] = P[0];

    //cout << fixed << setprecision(0) << "0: " << floor_to_double(minrad) << endl;
    for(long i = 1; i < n; i++){
        P2[i] = P2[i-1];
        Min_circle mc(P2, P2+n, true);
        c = mc.circle();
        rad = c.squared_radius();
        P2[i] = P[i];
        if(rad < minrad){
            minrad = rad;
        }
        //cout << fixed << setprecision(0) << i  << ": " << floor_to_double(minrad) << endl;
    }
*/
    cout << fixed << setprecision(0) << floor_to_double(CGAL::sqrt(minrad)) << endl;

}

int main() {
    long n; cin >> n;
    while(n){
        testcasefunction(n);
        cin >> n;
    }
}