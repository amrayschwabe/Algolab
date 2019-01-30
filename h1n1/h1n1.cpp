// Includes
// CGAL includes
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <climits>
#include <cassert>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
boost::no_property,				// interior properties of vertices
boost::property<boost::edge_weight_t, long> 		// interior properties of edges
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
typedef Triangulation::Face_iterator Face_iterator;
typedef Triangulation::Face_handle Face_handle;

typedef std::vector<long> VL;

//long MAX = std::numeric_limits<long>::max()/4;
long MAX = pow(2,49);
using namespace std;



long find(int info, std::vector<Vertex>& primpredmap, VL& sol, Graph& g){
    //go up tree
    if((sol[info] ) != (-1)){
        return sol[info];
    }else{
        Edge e; bool success;
        boost::tie(e, success) = boost::edge(info, primpredmap[info], g);
        long min = (long)std::min(MAX-get(boost::edge_weight_t(), g, e), find(primpredmap[info], primpredmap, sol, g));
        return min;
    }
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

        //label faces with ints starting from one, so that outside can be 0
        int V = 1;
        for (Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f) f->info() = V++;

        //create Graph
        Graph G(V);	// creates an empty graph on n vertices

        //iterate over faces, add all connections as edge to graph
        for(auto f= t.finite_faces_begin(); f!=t.finite_faces_end(); ++f){
            for(int i = 0; i<3; ++i){
                Vertex u = f->info();
                Vertex v;
                if (t.is_infinite(f->neighbor(i))){
                    v = 0;
                }else{
                    v = f->neighbor(i)->info();
                }
                long weight = (MAX - CGAL::squared_distance(f->vertex((i+1)%3)->point(), f->vertex((i+2)%3)->point()));
                add_edge(u, v, weight, G);
            }
        }
        //create prim minimum spanning tree
        std::vector<Vertex> primpredmap(V);     // We MUST use this vector as an Exterior Property Map: Vertex -> Prim Predecessor
        Vertex start = 0;
        boost::prim_minimum_spanning_tree(G, boost::make_iterator_property_map(primpredmap.begin(), boost::get(boost::vertex_index, G)),        // Prim from user-defined start,
                                          boost::root_vertex(start));

        //init sol array
        VL sol(V, -1);
        sol[0] = MAX;


        //read number of people that want to escape
        double nr_escape;
        cin >> nr_escape;
        for(double i = 0; i<nr_escape; ++i){
            K::Point_2 p;
            cin >> p;
            double min_distance_squared;
            cin >> min_distance_squared;

            //check whether condition is violated from the beginning
            K::Point_2 nearest= t.nearest_vertex(p)->point();
            double curr_distance_squared = CGAL::squared_distance(nearest, p);

            if(curr_distance_squared < min_distance_squared){
                cout << "n";
                continue;
            }

            //find face in which our escapee is located
            // find the face handle of our escapee
            Triangulation::Face_handle face_h = t.locate(p);

            if(t.is_infinite(face_h) || (find(face_h->info(), primpredmap, sol, G)>=4*min_distance_squared)) { // because distance on both sides & squared
                cout << "y";
            } else {
                cout << "n";
            }
        }
        cout << endl;


    }
}