#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_data_structure_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef std::pair<int,bool> info_t;
typedef CGAL::Triangulation_vertex_base_with_info_2<info_t,K> vertex_t;
typedef CGAL::Triangulation_face_base_2<K> face_t;
typedef CGAL::Triangulation_data_structure_2<vertex_t,face_t> triangulation_t;
typedef CGAL::Delaunay_triangulation_2<K,triangulation_t> delaunay_t;
typedef delaunay_t::Vertex_handle vhandle_t;
typedef delaunay_t::Vertex_circulator vcirculator_t;
bool has_interference(delaunay_t const & trg, K::FT const & rr) {
    for (auto e = trg.finite_edges_begin(); e != trg.finite_edges_end(); ++e) if (trg.segment(*e).squared_length() <= rr) return true;
    return false;
}
bool try_two_color(delaunay_t & trg, K::FT const & rr) {
    for (auto v = trg.finite_vertices_begin(); v != trg.finite_vertices_end(); ++v) v->info() = { 0, false };
    int components = 0;
    delaunay_t trg1, trg0;
    for (auto v = trg.finite_vertices_begin(); v != trg.finite_vertices_end(); ++v) {
        if (v->info().first == 0) {
            v->info().first = ++components;
            std::vector<vhandle_t> stack(1, v);
            do {
                vhandle_t h = stack.back();
                stack.pop_back();
                vcirculator_t c = trg.incident_vertices(h);
                do
                    if (!trg.is_infinite(c) && CGAL::squared_distance(h->point(), c->point()) <= rr)
                    {
                        if (c->info() == h->info()) return false;
                        if (c->info().first == 0) {
                            stack.push_back(c);
                            c->info() = { components, !h->info().second };
                        }
                    }
                while (++c != trg.incident_vertices(h));
            } while (!stack.empty());
        }
        if (v->info().second) trg1.insert(v->point());
        else trg0.insert(v->point());
    }
    return !has_interference(trg0, rr) && !has_interference(trg1, rr);
}
void test_case()
{
    std::size_t n, m; K::FT rr;
    std::cin >> n >> m >> rr;
    rr *= rr;
    std::vector<K::Point_2> stations(n);
    for (std::size_t i = 0; i < n; ++i) std::cin >> stations[i]; delaunay_t trg(stations.begin(), stations.end());
    bool success = try_two_color(trg, rr);
    for (std::size_t i = 0; i < m; ++i) {
        K::Point_2 holmes, watson;
        std::cin >> holmes >> watson;
        if (success) {
            if (CGAL::squared_distance(holmes, watson) <= rr) {
                std::cout << "y";
                continue;
            }
            auto station_holmes = trg.nearest_vertex(holmes);
            auto station_watson = trg.nearest_vertex(watson);
            if (station_holmes->info().first == station_watson->info().first &&
                CGAL::squared_distance(holmes, station_holmes->point()) <= rr &&
                CGAL::squared_distance(watson, station_watson->point()) <= rr) { std::cout << "y"; continue; }
        }
        std::cout << "n";
    }
    std::cout << "\n";
}
int main() {
    std::ios_base::sync_with_stdio(false); std::size_t k;
    std::cin >> k;
    for (int i = 0; i < k; ++i) test_case();
}
