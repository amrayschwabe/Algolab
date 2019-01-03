#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <vector>

// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
    boost::no_property,				// interior properties of vertices	
    boost::property<boost::edge_weight_t, double> 		// interior properties of edges
>					Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef boost::graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<long, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Face_iterator Face_iterator;
typedef Triangulation::Face_handle Face_handle;
using namespace std;
using namespace boost;

double MAX = pow(2, 50);

struct balloon {
    K::Point_2 point;
    long long radius;
};
void testcase(){
    int nr_trees; int nr_balloons; long long radius_trees;
    cin >> nr_trees >> nr_balloons >> radius_trees;

    vector<K::Point_2> trees;
    for (int i = 0; i < nr_trees; ++i) {
        K::Point_2 newt;
        cin >> newt;
        trees.push_back(newt);
    }

    vector<balloon> balloons;
    for (int i = 0; i < nr_balloons; ++i) {
        balloon newt;
        cin >> newt.point >> newt.radius;
        balloons.push_back(newt);
    }
   
    // construct triangulation
    Triangulation t;
    t.insert(trees.begin(), trees.end());

    //give id's to faces
    int V = 1;
    for (Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f) {
        f->info() = V++;
    }

    //construct graph
    Graph G(t.number_of_faces());
    for (Face_iterator e = t.finite_faces_begin(); e != t.finite_faces_end(); ++e) {
        for (int i = 0; i < 3; ++i) {
            Vertex u = e->info();
            Vertex v;
            if (t.is_infinite(e->neighbor(i))) {
                v = 0;
            }
            else {
                v = e->neighbor(i)->info();
            }
            double weight = (MAX - sqrt(CGAL::squared_distance(e->vertex((i + 1) % 3)->point(), e->vertex((i + 2) % 3)->point())));
            add_edge(u, v, weight, G);
        }
    }

    // Prim minimum spanning tree
    std::vector<Vertex> primpredmap(V+1);	// We MUST use this vector as an Exterior Property Map: Vertex -> Prim Predecessors
    boost::prim_minimum_spanning_tree(G, boost::make_iterator_property_map(primpredmap.begin(), boost::get(boost::vertex_index, G)));

    for(int i = 0; i < primpredmap.size(); ++i){
        cout << "index " << i << "value " << primpredmap[i] <<  endl;
    }
    //iterate over balloons
    for (int i = 0; i < nr_balloons; ++i) {
        //can balloon be inflated?
        K::Point_2 nearest = t.nearest_vertex(balloons[i].point)->point();
        if (CGAL::squared_distance(nearest, balloons[i].point) < (pow(balloons[i].radius+radius_trees,2))) {
            cout << "n";
            continue;
        }
        int start = t.locate(balloons[i].point)->info();
        int done = 0;
        int count = 0;
        while(primpredmap[start] != 0 && done == 0 && count < 10){
            Edge e; bool success;
        boost:tie(e, success) = boost::edge(start, primpredmap[start], G);
            //cout << "space " << MAX - get(boost::edge_weight_t(), G, e) - 2* radius_trees << " needed " <<  2 * balloons[i].radius << endl;
            if (MAX - get(boost::edge_weight_t(), G, e) - 2 * radius_trees > 2 * balloons[i].radius) {
                //cout << "start " << start << endl;
                start = primpredmap[start];
               // cout << "newstart " << start << endl;
                ++count;
            }
            else {
                cout << "n";
                done = 1;
            }
        }
        if (done == 0) {
            cout << "y";
        }
    }
    cout << endl;
}
int main()
{
    int t;
    cin >> t;
    while (t--) {
        testcase();
    }
}

