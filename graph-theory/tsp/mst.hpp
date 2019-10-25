#ifndef MST_HPP
#define MST_HPP

#include <string>
#include <algorithm>
#include <queue>

#include "common.hpp"

/** mst.hpp -- MST solver 
 * Find the minimal spanning tree of a complete graph.
 * Uses Kruskal's algorithm, with Union-Find.
 * or Prim's algorithm, with a priority queue.
 *
 */

///////////////////////////////////////////////////////////////////
////////// MST API ////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////



/** 
 * Finds a MST with Kruskal's algorithm, using DisjointSets (Union-Find).
 * N - number of vertices
 * dist - a distance matrix (N-by-N)
 * ds - an initialised disjoint sets object (space reasons)
 * 
 */
void kruskal(const distance_matrix & dist, int N,
             const std::vector<Point> & points,
             // const adjacency_list & adjlist,
             adjacency_list & mst,
             int* root
             );


/** 
 * Finds a MST with Prim's algorithm, using a priority queue (heap).
 * N - number of vertices
 * dist - a distance matrix (N-by-N)
 * 
 */
void prim(const distance_matrix & dist, int N, 
          const std::vector<Point> & points,
          const adjacency_list & adjlist,
          adjacency_list & mst,
          int* root
          );


///////////////////////////////////////////////////////////////////
////////// KruskalSolver //////////////////////////////////////////
///////////////////////////////////////////////////////////////////


class KruskalSolver {
    class DisjointSets {
        int n;
        std::vector<int> roots;
        std::vector<int> parents;
        // the result: a *directed* rooted tree
        adjacency_list & mst;

        int follow(int a); 
    public:
        DisjointSets(int n, adjacency_list & mst);
        bool same(int a, int b); 
        void join(int a, int b);
        // retrieve tree
        int root();
    };

    struct kruskal_edge {
        vertex first,second;
        // DIST d;
        // bool operator<(const kruskal_edge & other) {
        //     return d < other.d;
        // }
    };

    struct small_edge {
        int id;
        DIST d;
        bool operator<(const small_edge & other) const {
            return d < other.d;
        }
    };

    typedef std::pair<vertex, vertex> simple_edge;

    // typedef kruskal_edge edge;


    // constructor arguments
    const distance_matrix & dist;
    int N;
    const std::vector<Point> & points;
    // const adjacency_list & adjlist;
    adjacency_list & mst;
    int* root;
    // own attributes
    DisjointSets ds;
    std::vector<simple_edge> edges;

    
public:
    KruskalSolver(const distance_matrix & dist, int N,
                  const std::vector<Point> & points,
                  // const adjacency_list & adjlist,
                  adjacency_list & mst,
                  int* root);

    void setup();
    void solve();
private:

    void init_edges(int N, std::vector<simple_edge> & edges,
                    const distance_matrix & dist);
    void init_edges_v2(int N, std::vector<kruskal_edge> & edges,
                       const distance_matrix & dist);
    void union_find(DisjointSets & ds, 
                    std::vector<simple_edge> & edges);
    void union_find_v2(DisjointSets & ds, 
                       std::vector<kruskal_edge> & edges);

    void sort_edges(std::vector<small_edge> & id_edges);


};
    


///////////////////////////////////////////////////////////////////
////////// PrimSolver /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////


class PrimSolver {
    typedef std::pair<vertex, DIST> prioq_edge;

    struct prioq_lt {
        bool operator() (const prioq_edge & lhs, const prioq_edge & rhs) const {
            return lhs.second > rhs.second; // we want smallest on top, hence >
        }
    };

    typedef std::priority_queue<prioq_edge, std::vector<prioq_edge>, prioq_lt > edge_priority_queue;

    const int not_adjacent = -1; // not reachable or already visited

    // constructor arguments
    const distance_matrix & dist;
    int N;
    const std::vector<Point> & points;
    const adjacency_list & adjlist;
    adjacency_list & mst;
    vertex* root;

    // internal attributes
    std::vector<bool> visited;
    std::vector<DIST> shortest_distance;
    std::vector<vertex> cheapest_edge;
    // cheapest_edge[v] is the visited node closest to v
    edge_priority_queue Q;
    
public:
    PrimSolver(const distance_matrix & dist, int N, 
               const std::vector<Point> & points,
               const adjacency_list & adjlist,
               adjacency_list & mst,
               int* root);
    void setup();
    void solve();
private:
    void expand_adjacent(vertex curr); 

};




#endif // MST_HPP
