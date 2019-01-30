//NOT WORKING

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <CGAL/Circle_2.h>
#include <climits>
#include <cassert>


typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::FT FT;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<std::pair<FT ,long >,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>  Triangulation;
typedef Triangulation::Finite_faces_iterator  Face_iterator;
typedef Triangulation::All_faces_iterator  All_face_iterator;
typedef Triangulation::Edge_iterator  Edge_iterator;


typedef  CGAL::Min_circle_2_traits_2<K>  Traits;
typedef  CGAL::Min_circle_2<Traits>      Min_circle;
using namespace std;


//boost stuff
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/max_cardinality_matching.hpp>
#include <boost/multiprecision/cpp_int.hpp>
using boost::multiprecision::cpp_int;

// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
boost::no_property,				// interior properties of vertices
boost::property<boost::edge_weight_t, FT > 		// interior properties of edges
>					Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an FT  in the range [0, num_vertices(G)).
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef boost::graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t

using namespace boost;

FT MAX = FT(LONG_MAX)*FT(LONG_MAX);
void testcase(){

    long nr_trees, nr_balloons;
    FT radius;
    cin >> nr_trees >> nr_balloons >> radius;

    vector<K::Point_2> trees(nr_trees);
    for(int  i = 0; i < nr_trees; ++i){
        cin >> trees[i];
    }
    // construct triangulation
    Triangulation t;
    t.insert(trees.begin(), trees.end());

    vector<FT > infomap;
    /*
    for (All_face_iterator f = t.all_faces_begin(); f != t.all_faces_end(); ++f){
        f->info().second = -1;
    }*/
    //iterate over faces and add largest circle info to each
    long count = 0;
    for (Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f){
        /*vector<K::Point_2> triangle_vertices(3);
        for(int  i = 0; i < 3; ++i){
            triangle_vertices[i] = t.triangle(f).vertex(i);
        }*/
        //CGAL::set_pretty_mode( std::cout);
        //CGAL::Min_circle_2<CGAL::Min_circle_2_traits_2<CGAL::Epick> >::Circle circle = Min_circle(triangle_vertices.begin(), triangle_vertices.end(), true).circle();
        //cout << circle.center() << " " << circle.squared_radius() << endl;
        //f->info() = make_pair((circle.squared_radius()), count);
        FT escape = CGAL::squared_radius(f->vertex(0)->point(), f->vertex(1)->point(), f->vertex(2)->point());
        f->info() = make_pair(escape, count);
        infomap.push_back((escape));
        count ++;
        //cout << "count " << count << endl;
        //cout << "circle squared radius  " << circle.squared_radius() << endl;
    }
    infomap.push_back(MAX);
    //create graph of connections between faces
    Graph G(count+1);	// creates an empty graph on n vertices
    WeightMap weightmap = boost::get(boost::edge_weight, G);
    //WeightMap weightmap = boost::get(boost::edge_weight, G);// start by defining property maps for all interior properties defined in Lines 37, 38
    for (Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f){
        long mylabel = f->info().second;
        //cout << "mylabel " << mylabel << endl;
        long otherlabel;
        for(int  i = 0; i < 3; ++i){
            if (t.is_infinite(f->neighbor(i))){
                otherlabel = count;
            }else{
                otherlabel = f->neighbor(i) -> info().second;
            }
            FT edgeweight = (CGAL::squared_distance(f->vertex((i+3-1)%3)->point(), f->vertex((i+3-2)%3)->point()));
            //FT escape = CGAL::squared_radius(f->vertex(0)->point(), f->vertex(1)->point(), f->vertex(2)->point());
            FT escape = infomap[mylabel];
            //cout << "edgeweight " << edgeweight << endl;
            Edge e; bool success;
            tie(e, success) = add_edge(mylabel, otherlabel,G);
            weightmap[e] = -edgeweight;
            tie(e,success) = add_edge(mylabel, count, G);
            weightmap[e] = -escape;
        }
    }
    std::vector < Edge > spanning_tree;
    kruskal_minimum_spanning_tree(G, std::back_inserter(spanning_tree));
    //construct ned tree from minimum spanning tree
    Graph G2(count+1);
    WeightMap weightmap2 = boost::get(boost::edge_weight, G2);
    for (std::vector < Edge >::iterator ei = spanning_tree.begin();
         ei != spanning_tree.end(); ++ei) {
        Edge e; bool success;
        boost::tie(e, success) = boost::edge(source(*ei,G), target(*ei,G),G);
        FT weight = weightmap[e];
        boost::tie(e,success) = add_edge(source(*ei,G), target(*ei,G),G2);
        weightmap2[e] = -weight;
        //cout << "added edge from " << source(*ei,G)<< " to " << target(*ei,G) << " with weight " << -weight << endl;
    }


    // read in balloons
    vector<pair<K::Point_2, FT > >balloons(nr_balloons);
    //iterate over min spanning tree

    for (std::size_t i = 0; i < nr_balloons; ++i) {
        K::Point_2 p;
        std::cin >> p;
        FT radius;
        cin >> radius;
        balloons[i] = make_pair(p, radius);
    }

    //iterate over spanning tree and add biggest balloon that can escape
    std::vector<bool> vis(count+1, false); // visited flags
    std::queue<long > Q; // BFS queue (from std:: not boost::)
    vis[count] = true; // Mark the source as visited
    Q.push(count);
    vector<FT> sol(count+1,-1);
    sol[count] = MAX;
    while (!Q.empty()) {
        long u = Q.front();
        assert(u < count+1);
        Q.pop();
        OutEdgeIt ebeg, eend;
        FT sol_u = sol[u];
        for (boost::tie(ebeg, eend) = boost::out_edges(u, G2); ebeg != eend; ++ebeg) {
            long  v = boost::target(*ebeg, G2);
            assert(v < count+1);
            // Only follow edges with spare capacity
            if (vis[v]) continue;
            //cout << "infomap " << infomap[current_index] << endl;
            FT weight = weightmap2[*ebeg];
            sol[v] = min(sol_u, weight);
            vis[v] = true;
            Q.push(v);
        }
    }

    //iterate over balloons
    for(long i = 0; i < nr_balloons; ++i){
        //check whether inflatable
        K::Point_2 current_point = balloons[i].first;
        K::Point_2 nearest_vertex = t.nearest_vertex(current_point)->point();
        FT current_radius = balloons[i].second;
        FT mindistance = ((current_radius + radius) * 2)*((current_radius + radius) * 2);

        if((CGAL::squared_distance(current_point, nearest_vertex)) < (FT(current_radius + radius)*FT(current_radius + radius))){
            //cout << "not inflatable ";
            cout << "n";
            continue;
        }
        auto face = t.inexact_locate(current_point);
        long current_index = face->info().second;
        if(t.is_infinite(face)){
            cout << "y";
            continue;
        }
        //else check in sol if it can be moved to starting point
        if(sol[current_index] < mindistance){
            cout << "n";
        }else{
            cout << "y";
        }
        //cout << "balloon " << i << endl;*/
    }
    cout << endl;

}
int main()
{
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        testcase();
    }
}
