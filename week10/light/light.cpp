
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/basic.h>
#include <CGAL/squared_distance_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Point_set_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef K::Point_2 Point;
typedef K::Circle_2 Circle;

typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_data_structure_2<Vb> Tds;
typedef CGAL::Point_set_2<K, Tds> PSet;
typedef CGAL::Point_set_2<K, Tds>::Vertex_handle Vertex_handle;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;

using namespace std;

int main() {
    // some basic setup stuff
    cin.sync_with_stdio(false);
    cout.sync_with_stdio(false);
    cout << fixed << setprecision(0);

    int test_count;
    cin >> test_count;

    for(int test = 0; test < test_count; test++) {
        int nr_participants, nr_lamps;
        cin >> nr_participants >> nr_lamps;

        //---------------------------------
        //read in x y r for each participant, add to vector<participant>
        vector<K::Point_2> partvec(nr_participants);
        vector<int> radvec(nr_participants);
        for(int i = 0; i < nr_participants; ++i){
            K::Point_2 p;
            int r;
            cin >> p >> r;
            partvec[i] = p;
            radvec[i] = r;
        }

        //read in height = radius
        int lampradius;
        cin >> lampradius;

        //read in lights as vector<lamp>
        vector<K::Point_2> lampvec(nr_lamps);
        for(int i = 0; i < nr_lamps; ++i){
            K::Point_2 p;
            cin >> p;
            lampvec[i] = p;
        }

        //calculate triangulation
        Triangulation t;
        t.insert(lampvec.begin(), lampvec.end());
        //store for each person by which light it gets hit first
        vector<int>firsthit(nr_participants, numeric_limits<int>::max());
        vector<int> survivors;
        int longestsurvivor = 0;
        //iterate over all participants
        for(int i = 0; i < nr_participants; ++i) {
            //participant location
            K::Point_2 loc = partvec[i];
            K::Point_2 nearestneighbor = t.nearest_vertex(loc)->point();
            double maxdist = pow(radvec[i] + lampradius,2);
            //if closest lamp far away, person survives
            if (CGAL::squared_distance(loc, nearestneighbor) >= maxdist) {
                survivors.push_back(i);
            }
                //else have to check for lamp that hits first
            else {
                for (int j = 0; j < nr_lamps; ++j) {
                    if (maxdist > CGAL::squared_distance(lampvec[j], loc)) {
                        firsthit[i] = j;
                        longestsurvivor = max(longestsurvivor, j);
                        break;
                    }
                }
            }
        }
        if(survivors.size() > 0){
            for(int i:survivors){
                cout << i << " ";
            }
        }else{
            for(int i = 0; i < nr_participants; ++i){
                if (firsthit[i] == longestsurvivor){
                    cout << i << " ";
                }
            }
        }

        cout << endl;

}
