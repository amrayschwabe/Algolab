#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/intersections.h>
#include <CGAL/squared_distance_2.h>

using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

double floor_to_double(const K::FT& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}


void testcase(int n) {
    long x, y, a, b;
    cin >> x;
    cin >> y;
    cin >> a;
    cin >> b;

    K::Point_2 o(x, y);
    K::Point_2 d(a, b);

    K::Ray_2 r(o, d);

    K::Point_2 closestIntersection;
    K::FT minDistance = -1;

    while (n--) {
        long p1, p2, p3, p4;
        cin >> p1;
        cin >> p2;
        cin >> p3;
        cin >> p4;

        K::Point_2 sb(p1, p2);
        K::Point_2 se(p3, p4);

        K::Segment_2 s(sb, se);

        K::FT tmp_dist;

        if (CGAL::do_intersect(r, s)) {
            auto tmp = CGAL::intersection(s, r);
            if (const K::Point_2 *p = boost::get<K::Point_2>(&*tmp)) {
                K::FT tmp_dist = CGAL::squared_distance(o, *p);
                if (minDistance == -1 || tmp_dist < minDistance) {
                    minDistance = tmp_dist;
                    closestIntersection = *p;
                }
            } else if (const K::Segment_2 *s = boost::get<K::Segment_2>(&*tmp)) {
                tmp_dist = CGAL::squared_distance(o, s->source());
                if (minDistance == -1 || tmp_dist < minDistance) {
                    minDistance = tmp_dist;
                    closestIntersection = s->source();
                }
                tmp_dist = CGAL::squared_distance(o, s->target());
                if (minDistance == -1 || tmp_dist < minDistance) {
                    minDistance = tmp_dist;
                    closestIntersection = s->target();
                }
            }
        }
    }

        if (minDistance != -1) {
            cout << fixed << setprecision(0) << floor_to_double(closestIntersection.x()) << " " << floor_to_double(closestIntersection.y()) << endl;
        } else {
            cout << "no" << endl;
        }
}

int main() {
  int n; cin >> n;
  while (n) {
    testcase(n);
    cin >> n;
  }
}
