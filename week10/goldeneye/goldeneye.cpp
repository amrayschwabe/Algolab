#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <climits>
#include <set>
#include <map>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef boost::disjoint_sets_with_storage<> Uf; // Member

using namespace std;
struct Edge{
    K::Point_2 from;
    K::Point_2 to;
    K::FT distance_squared;
};

void testcase(){
    cout << fixed << setprecision(0);
    int nr_jammers, nr_missions;
    double initial_power;
    cin >>nr_jammers >> nr_missions >> initial_power;
    vector<K::Point_2> pts;

    //read in jammers
    map<K::Point_2, int> idmap;
    for(int i = 0; i < nr_jammers; ++i){
        K::Point_2 p;
        std::cin >> p;
        pts.push_back(p);
        idmap.insert(make_pair(p, i));
    }

    // construct triangulation
    Triangulation t;
    t.insert(pts.begin(), pts.end());

    //iterate over all edges and add length
    vector<Edge> edges;

    for(auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e){
        Edge newe;
        newe.from = t.segment(e).source();
        newe.to = t.segment(e).target();
        newe.distance_squared = t.segment(e).squared_length();
        edges.push_back(newe);
    }

    //sort edges by increasing length
    sort(edges.begin(), edges.end(), [](Edge rhs, Edge lhs){return rhs.distance_squared < lhs.distance_squared;});

    //create set and iterate over all edges to add the ones that are short enough
    Uf unionset(nr_jammers);
    int edgeslength = edges.size();
    for(int i = 0; i < edgeslength; ++i){
        if(edges[i].distance_squared <= initial_power){
            unionset.union_set(idmap[edges[i].from], idmap[edges[i].to]);
        }
    }

    Uf unionseta(nr_jammers);
    Uf unionsetb(nr_jammers);
    K::FT minradius = 0;
    int minradiusit = 0;
    K::FT minradiusb = 0;
    int minradiusbit = 0;
    //add all edges <= initial power to union find structure
    for(int i = 0; i < nr_missions; ++i){

        //read in mission
        K::Point_2 from; K::Point_2 to;
        cin >> from >> to;

        //calculate distance to next jammer of from and to point
        K::Point_2 from_nearest = t.nearest_vertex(from)->point();
        K::Point_2 to_nearest = t.nearest_vertex(to)->point();
        K::FT from_nearest_distance = CGAL::squared_distance(from, from_nearest);
        K::FT to_nearest_distance = CGAL::squared_distance(to, to_nearest);
        K::FT d = 4 * max(from_nearest_distance, to_nearest_distance);


        if((d <= initial_power)&& (unionset.find_set(idmap[from_nearest]) == unionset.find_set(idmap[to_nearest]))){
            cout << "y";
            //cout << "d " << d << endl;
            if(d >= minradius) {
                minradius = d;
            }
            for(;(minradiusit < edgeslength)&&(unionseta.find_set(idmap[from_nearest]) != unionseta.find_set(idmap[to_nearest])); ++minradiusit){
                unionseta.union_set(idmap[edges[minradiusit].from], idmap[edges[minradiusit].to]);
            }

        }else{
            cout << "n";
            if(d >= minradiusb) {
                minradiusb = d;
            }
            for(;(minradiusbit < edgeslength)&&(unionsetb.find_set(idmap[from_nearest]) != unionsetb.find_set(idmap[to_nearest])); ++minradiusbit){
                unionsetb.union_set(idmap[edges[minradiusbit].from], idmap[edges[minradiusbit].to]);
            }

        }

    }
    if(minradiusit != 0 && edges[minradiusit-1].distance_squared > minradius){
        minradius = edges[minradiusit-1].distance_squared;
    }
    if(minradiusbit != 0 && edges[minradiusbit-1].distance_squared > minradiusb){
        minradiusb = edges[minradiusbit-1].distance_squared;
    }
    if(minradiusb < minradius){
        minradiusb = minradius;
    }
    cout << "\n" << minradiusb << "\n" << minradius << endl;

}
// Main
int main() {
    ios_base::sync_with_stdio(false);
    long nr;
    cin >> nr;
    while(nr--){
        testcase();
    }
}