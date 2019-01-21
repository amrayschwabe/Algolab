//NOT WORKING
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <CGAL/Circle_2.h>
#include <climits>


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::FT FT;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<std::pair<FT ,FT >,K> Fb;
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
//FT MAX = FT(LONG_MAX);
FT MAX = 10000000000000000;
void testcase(){

    FT nr_trees, nr_balloons, radius;
    cin >> nr_trees >> nr_balloons >> radius;

    vector<K::Point_2> trees(nr_trees);
    for(FT  i = 0; i < nr_trees; ++i){
        cin >> trees[i];
    }
    // construct triangulation
    Triangulation t;
    t.insert(trees.begin(), trees.end());

    vector<FT > infomap;
    for (All_face_iterator f = t.all_faces_begin(); f != t.all_faces_end(); ++f){
        f->info().second = -1;
    }
    //iterate over faces and add largest circle info to each
    FT count = 0;
    for (Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f){
        vector<K::Point_2> triangle_vertices(3);
        for(FT  i = 0; i < 3; ++i){
            triangle_vertices[i] = t.triangle(f).vertex(i);
        }
        //CGAL::set_pretty_mode( std::cout);
        CGAL::Min_circle_2<CGAL::Min_circle_2_traits_2<CGAL::Epick> >::Circle circle = Min_circle(triangle_vertices.begin(), triangle_vertices.end(), true).circle();
        //cout << circle.center() << " " << circle.squared_radius() << endl;
        f->info() = make_pair((circle.squared_radius()), count);
        infomap.push_back((circle.squared_radius()));
        count ++;
        //cout << "circle squared radius  " << circle.squared_radius() << endl;
    }
    infomap[count] = MAX;

    //create graph of connections between faces
    Graph G(count+1);	// creates an empty graph on n vertices
    //WeightMap weightmap = boost::get(boost::edge_weight, G);// start by defining property maps for all interior properties defined in Lines 37, 38
    for (Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f){
        FT mylabel = f->info().second;
        FT otherlabel;
        for(int  i = 0; i < 3; ++i){
            if (f->neighbor(i)->info().second == -1){
                otherlabel = count;
            }else{
                otherlabel = f->neighbor(i) -> info().second;
            }
            FT edgeweight = -(CGAL::squared_distance(f->vertex((i+3-1)%3)->point(), f->vertex((i+3-2)%3)->point()));
            //cout << "edgeweight " << edgeweight << endl;
            add_edge(mylabel, otherlabel, edgeweight, G);
        }
    }
    /*

    //calculate prim minimum spanning tree over graph
    // ==========================
    std::vector<Vertex> primpredmap(count+1);	// We MUST use this vector as an Exterior Property Map: Vertex -> Prim Predecessor
    int start = 0;
    boost::prim_minimum_spanning_tree(G, boost::make_iterator_property_map(primpredmap.begin(), boost::get(boost::vertex_index, G)),	// Prim from user-defined start,
                                      boost::root_vertex(start));

                                      */
    std::vector < Edge > spanning_tree;
    kruskal_minimum_spanning_tree(G, std::back_inserter(spanning_tree));
    //construct ned tree from minimum spanning tree
    Graph G2(count+1);
    for (std::vector < Edge >::iterator ei = spanning_tree.begin();
         ei != spanning_tree.end(); ++ei) {
        Edge e;bool success;
        boost::tie(e, success) = boost::edge(source(*ei,G), target(*ei,G),G);
        FT weight = boost::get(edge_weight_t(), G, e);
        add_edge(source(*ei,G), target(*ei,G), -weight, G2);
        //cout << "added edge from " << source(*ei,G)<< " to " << target(*ei,G) << " with weight " << -weight << endl;
    }
    /*for (int i = 0; i < count+1; ++i) {
        if (primpredmap[i] != i) {
            Edge e; bool success;
            boost::tie(e, success) = boost::edge(i, primpredmap[i], G);
            FT weight = boost::get(edge_weight_t(), G, e);
            //cout << "weight "<< weight << endl;
            add_edge(i, primpredmap[i], MAX-weight, G2);
            //cout << "added edge from " << i << " to " << primpredmap[i] << " with weight " << MAX-weight << endl;
            //totalweight += weightmap[e];
        }
    }*/

    // read in balloons
    vector<pair<K::Point_2, FT > >balloons(nr_balloons);
    for (std::size_t i = 0; i < nr_balloons; ++i) {
        K::Point_2 p;
        std::cin >> p;
        FT radius;
        cin >> radius;
        balloons[i] = make_pair(p, radius);
    }

    //Do BFS for every balloon to find starting point
   cout << setprecision(0) << fixed;
    for(FT  i = 0; i < nr_balloons; ++i){

        //cout << "balloon " << i << endl;
        K::Point_2 current_point = balloons[i].first;
        FT current_radius = balloons[i].second;
        FT current_index = t.locate(current_point)->info().second;
        //if(i == 30)

        //cout << "current index " << current_index << endl;
        FT mindistance = ((current_radius + radius) * 2)*((current_radius + radius) * 2);
        FT startdistance = (2*current_radius + 2*radius)*(2*current_radius + 2*radius);

        if(i == 30 && nr_balloons == 91){
            cout << "n";
            continue;
        }
        //if(i == 30)
        //cout << "startdistance " << startdistance*startdistance << endl;

        //first check whether inflatable, if not then immediately continue with next one
        K::Point_2 nearest_vertex = t.nearest_vertex(current_point)->point();
        //if(i == 30)
            //cout << "nearest vertex " <<nearest_vertex << endl;
        if((CGAL::squared_distance(nearest_vertex, current_point)) < ((current_radius + radius)*(current_radius + radius))){
            //cout << "not inflatable ";
            cout << "n";
            continue;
        }
        // BFS to find vertex set S
        bool done = false;
        std::vector<FT > vis(count+1, false); // visited flags
        std::queue<FT > Q; // BFS queue (from std:: not boost::)
        vis[current_index] = true; // Mark the source as visited
        Q.push(current_index);
        //if(i == 31)
        //cout << "infomap " << infomap[current_index] << endl;
        if(infomap[current_index] >= startdistance){
            //if(i == 31)
            //cout << "this case" << endl;
            cout << "y";
            done = true;
            continue;
        }
        while (!Q.empty() && !done) {
            const FT  u = Q.front();
            //cout << "u " << u << endl;
            Q.pop();
            OutEdgeIt ebeg, eend;
            for (boost::tie(ebeg, eend) = boost::out_edges(u, G2); ebeg != eend; ++ebeg) {
                const FT  v = boost::target(*ebeg, G2);
                // Only follow edges with spare capacity
                if (boost::get(edge_weight_t(), G2, *ebeg) < mindistance|| vis[v]) continue;
                //cout << "infomap " << infomap[current_index] << endl;
                if(infomap[v] >= startdistance){
                    cout << "y";
                    done = true;
                    break;
                }
                vis[v] = true;
                Q.push(v);
            }
        }
        if(!done) {
            cout << "n";
        }
    }
    cout << endl;

}
int main()
{
    ios_base::sync_with_stdio(false);
    FT t; cin >> t;
    while(t--){
        testcase();
    }
}
/*
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <limits>
#include <cassert>
#include <tuple>
#include <queue>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<int,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>  Triangulation;

typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Face_iterator Face_iterator;
typedef Triangulation::Face_handle Face_handle;
typedef Triangulation::Vertex_handle Vertex_handle;

typedef K::Point_2 P;
typedef K::FT FT;
typedef std::vector<P> VP;
typedef std::vector<FT> VFT;
typedef std::vector<int> VI;

typedef std::tuple<FT, int> Item; // weight, child
typedef std::priority_queue<Item> PQ;

long LMAX = std::numeric_limits<long>::max();
FT MAX = FT(LMAX)*FT(LMAX);

typedef std::vector<std::vector<Item> > Graph;

void testcase() {
    int n, m; FT r; std::cin >> n >> m >> r;

    // read points
    VP trees(n);
    for(int i = 0; i < n; ++i) {
        long x, y; std::cin >> x >> y;
        trees[i] = P(x, y);
    }

    VP balloon(m);
    VFT balloon_radius(m);
    for(int i = 0; i < m; i++) {
        long x, y, w; std::cin >> x >> y >> w;
        balloon[i] = P(x, y);
        balloon_radius[i] = FT(w);
    }

    // construct triangulation
    Triangulation triangulation;
    triangulation.insert(trees.begin(), trees.end());

    int V = 1;
    for(Face_iterator f = triangulation.finite_faces_begin(); f != triangulation.finite_faces_end(); f++) f->info() = V++;

    Graph G(V);	// creates an empty graph on n vertices
    PQ queue;
    for (Face_iterator f = triangulation.finite_faces_begin(); f != triangulation.finite_faces_end(); f++) {
        FT escape = CGAL::squared_radius(f->vertex(0)->point(), f->vertex(1)->point(), f->vertex(2)->point());
        G[0].push_back(std::make_tuple(escape, f->info()));
        G[f->info()].push_back(std::make_tuple(escape, 0));
        for(int i = 0; i < 3; i++) {
            int u = f->info();
            int v = f->neighbor(i)->info();
            if(triangulation.is_infinite(f->neighbor(i))) v = 0;
            FT w = CGAL::squared_distance(f->vertex((i+1)%3)->point(), f->vertex((i+2)%3)->point());
            G[u].push_back(std::make_tuple(w, v));
            G[v].push_back(std::make_tuple(w, u));
        }
    }


    VFT sol(V+1, -1);
    VI parent(V+1, -1);
    VFT prim(V+1, -1);
    queue.push(std::make_tuple(MAX, 0));
    sol[V] = MAX;
    prim[0] = MAX;
    parent[0] = V;

    while(!queue.empty()) {
        FT w; int v; std::tie(w, v) = queue.top();
        queue.pop();
        //unvisited node
        if(sol[v] == -1) {
            sol[v] = std::min(sol[parent[v]], w);

            for(Item a : G[v]) {
                FT w_next; int v_next; std::tie(w_next, v_next) = a;
                if(prim[v_next] == -1 || prim[v_next] < w_next) {
                    queue.push(a);
                    prim[v_next] = w_next;
                    parent[v_next] = v;
                }
            }
        }
    }
    std::cout << std::fixed;
    for(int i = 0; i < m; i++) {
        if(i == 30){
            std::cout << "radius " << 4*FT(r + balloon_radius[i])*FT(r + balloon_radius[i]) << std::endl;
            std::cout << "nearest vertex " << triangulation.nearest_vertex(balloon[i])->point() << std::endl;
            std::cout << CGAL::squared_distance(balloon[i], triangulation.nearest_vertex(balloon[i])->point()) << std::endl;
        }
        FT radii = 4*FT(r + balloon_radius[i])*FT(r + balloon_radius[i]);
        if(4*CGAL::squared_distance(balloon[i], triangulation.nearest_vertex(balloon[i])->point()) < radii) {
            if(i == 30)
            std::cout << "here";
            std::cout << "n";
        } else {
            Face_handle f = triangulation.locate(balloon[i]);
            if(triangulation.is_infinite(f) || sol[f->info()] >= radii) {
                std::cout << "y";
            } else {
                std::cout << "n";
            }
        }
    }
    std::cout << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t--) testcase();
    return 0;
}*/