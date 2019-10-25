#ifndef CHRISTOFIDES_HPP
#define CHRISTOFIDES_HPP

#include <forward_list>
#include <list>
#include <memory>


#include "common.hpp"

/**
 * christofides.hpp -- A solver using Christofides' algorithm
 *
 * Created by Freddie on Fri Oct 28 18:40:58 2016
 */


void christofides_prim(int N, const distance_matrix & dist,
                       std::vector<int> & tour,
                       const std::vector<Point> & points);

void christofides_kruskal(int N, const distance_matrix & dist,
                          std::vector<int> & tour,
                          const std::vector<Point> & points);


class Christofides {
    struct tour_edge {
        vertex fst, snd;
        bool visited;
        tour_edge(vertex t, vertex f, bool v)
            : fst(t), snd(f), visited(v) {}
        vertex follow(vertex v) {
            // (if no assertion errors) change to:
            // return fst+snd-v;
            if (v == fst) return snd;
            else {
                assert (v == snd);
                return fst;
            }
        }
        tour_edge(const tour_edge &) = delete;
        tour_edge & operator=(const tour_edge &) = delete;
        tour_edge() = delete;
    };

    typedef std::vector<std::vector<tour_edge*>> tour_adjacency_list;
    typedef std::list<tour_edge*> tour_segment;

    // constructor arguments
    int N;
    const distance_matrix & dist;
    std::vector<vertex> & tour;
    const std::vector<Point> & points;
    
    // own attributes
    vertex root;
    adjacency_list mst;
    adjacency_list adjlist;
    std::vector<vertex> odds;
    tour_adjacency_list tour_adjlist;

public:
    
    Christofides(int N, const distance_matrix & dist,
                 std::vector<vertex> & tour,
                 const std::vector<Point> & points);


    void run_prim();
    void run_kruskal();
    void find_matching();
    void find_euler_tour();

private:
    void find_odds(int N, const adjacency_list & mst,
                   std::vector<vertex> & odds, vertex root);

    void greedy_matching(int N, const distance_matrix & dist,
                         const std::vector<vertex> & odds,
                         tour_adjacency_list & matching, DIST*weight=nullptr);

    void greedy_matching_v2(int N, const distance_matrix & dist,
                            const std::vector<vertex> & odds,
                            tour_adjacency_list & matching, DIST*weight=nullptr);

    void greedy_matching_x2(int N, const distance_matrix & dist,
                            const std::vector<vertex> & odds,
                            tour_adjacency_list & matching);

    
    void euler_tour(const tour_adjacency_list & adjlist,
                    tour_segment & result,
                    vertex root);

    void euler_tour_v2(tour_adjacency_list & adjlist,
                       tour_segment & result,
                       vertex root);

    
    void shortcutting(size_t N, const tour_segment & etour,
                      std::vector<vertex> & tour,
                      vertex root);

    void shortcutting_reverse(size_t N, const tour_segment & etour,
                              std::vector<vertex> & tour,
                              vertex root);

    void order_children(int N, int parent, int current,
                        tour_adjacency_list & adjlist,
                        const std::vector<Point> & points);

    double absval(Point p);

    double scalarprod(const Point & with, const Point & from, const Point & to);
    
};


#endif
