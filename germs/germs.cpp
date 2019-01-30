#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <climits>
#include <map>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Point Point;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_handle Vertex_handle;
typedef Triangulation::Face_handle Face_handle;
typedef Triangulation::Finite_vertices_iterator FVertexIt;
typedef Triangulation::Finite_edges_iterator FEdgeIt;


using namespace std;

double ceil_to_double(const K::FT& x) {
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a += 1;
    while (a-1 >= x) a -= 1;
    return a;
}


int main(){
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(0);
    while(true){
        // read number of infected people
        double nr;
        std::cin >> nr;
        if(nr < 1){
            return 0;
        }
        // read outlines of dish
        int l, b, r, t;
        cin >> l >> b >> r >> t;

        //read position of bacteria
        std::vector<K::Point_2> pts;
        pts.reserve(nr);
        for (std::size_t i = 0; i < nr; ++i) {
            K::Point_2 p;
            std::cin >> p;
            pts.push_back(p);

        }
        // construct triangulation
        Triangulation tri;
        tri.insert(pts.begin(), pts.end());

        //starting info is how far from edges
        // Find closest point to bounding box
        map<Point , double> distances;
        for (auto v = tri.finite_vertices_begin(); v != tri.finite_vertices_end(); ++v) {
            K::FT box = v->point().x() - l;
            box = min(box, r - v->point().x());
            box = min(box, v->point().y() - b);
            box = min(box, t - v->point().y());
            distances[v->point()] = CGAL::to_double(box * box);
        }

        // Find closest point to others
        for (FEdgeIt e = tri.finite_edges_begin(); e != tri.finite_edges_end(); ++e) {
            Vertex_handle v1 = e->first->vertex((e->second + 1) % 3);
            Vertex_handle v2 = e->first->vertex((e->second + 2) % 3);
            double distance = CGAL::to_double(CGAL::squared_distance(v1->point(), v2->point()) / 4);
            distances[v1->point()] = min(distances[v1->point()], distance);
            distances[v2->point()] = min(distances[v2->point()], distance);
        }

        //extract only distances
        vector<double> newdists;
        for(auto i:distances){
            newdists.push_back(i.second);
        }


        sort(newdists.begin(), newdists.end());

        auto deathtime = [newdists](int i){
            return max(0.0, ceil(sqrt(sqrt(newdists[i])-0.5)));
        };

        cout << deathtime(0) << ' ' << deathtime(nr/2) << ' ' << deathtime(nr-1) << endl;

    }
}
