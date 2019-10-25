#include "mst.hpp"
#include <iostream>


#define CERR_MST

/** mst.cpp -- MST solver 
 *
 */

///////////////////////////////////////////////////////////////////
////////// MST API ////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////


void kruskal(const distance_matrix & dist, int N, 
             const std::vector<Point> & points,
             // const adjacency_list & adjlist,
             adjacency_list & mst,
             vertex* root
             ) {
    KruskalSolver ks(dist, N, points, // adjlist,
                     mst, root);
    ks.setup();
    ks.solve();
}


void prim(const distance_matrix & dist, int N,
          const std::vector<Point> & points,
          const adjacency_list & adjlist,
          adjacency_list & mst,
          vertex* root
          )
{
    PrimSolver ps(dist, N, points, adjlist, mst, root);
    ps.setup();
    ps.solve();
}



///////////////////////////////////////////////////////////////////
////////// KruskalSolver //////////////////////////////////////////
///////////////////////////////////////////////////////////////////


int KruskalSolver::DisjointSets::follow(int a) {
    int curr = a;
    int nxt = roots[a];
    while (curr != nxt) {
        curr = nxt;
        nxt = roots[curr];
    }
    roots[a] = curr;
    return curr;
}


KruskalSolver::DisjointSets::DisjointSets(int n, adjacency_list & mst) :
    n(n),
    roots(n), 
    parents(n), 
    mst(mst)
{
    mst = adjacency_list(n, std::vector<int>(0));
    for (int i=0; i<n; i++) {
        roots[i] = i;
        parents[i] = i;
        mst[i].reserve(n);
    }
}

bool KruskalSolver::DisjointSets::same(int a, int b) {
    return (roots[a] == roots[b]) || (follow(a) == follow(b));
}

void KruskalSolver::DisjointSets::join(int a, int b) {
    int parent = follow(a),
        child = follow(b);
    roots[child] = parent;
    parents[child] = parent;
    mst[parent].push_back(child);
    #ifdef CERR_MST
    std::cerr << parent << " " << child << std::endl;
    #endif
}

int KruskalSolver::DisjointSets::root() {
    int res = -1;
    for (int i=0; i<n; i++) {
        // std::cerr << "parent of " << i << " is " << parents[i] << std::endl;
        if (parents[i] == i) {
            res = i;
            // std::cerr << "possible root " << i << std::endl;
        }
        // for (int child : adjlist[i] ) {
        //     std::cerr << "a child of " << i << " is " << child << std::endl;
        // }
    }
    assert( res != -1);
    #ifdef CERR_MST
    std::cerr << "Root: " << res << std::endl;
    #endif

    return res;
}

// DIST square_distance(Point a, Point b) {
//     double diffx = a.x - b.x;
//     double diffy = a.y - b.y;
//     double norm = diffx*diffx + diffy*diffy;
// #ifdef INT_DISTANCES
//     return lrint(norm);
// #else
//     return norm;
// #endif
// }


void KruskalSolver::union_find(DisjointSets & ds, 
                               std::vector<simple_edge> & edges) {
    // build MST and walk distance
    DIST walk = 0;
    for (simple_edge ab : edges) {
        int a = ab.first,
            b = ab.second;
        if (!ds.same(a,b)) {
            ds.join(a,b);
            walk += dist[a][b];
        }
    }
    std::cerr << "Size of MST (Kruskal): " << walk << std::endl;
}

void KruskalSolver::union_find_v2(DisjointSets & ds, 
                                  std::vector<kruskal_edge> & edges) {
    // build MST and walk distance
    for (kruskal_edge ab : edges) {
        int a = ab.first,
            b = ab.second;
        if (!ds.same(a,b)) {
            ds.join(a,b);
        }
    }
}


void KruskalSolver::init_edges(int N, std::vector<simple_edge> & edges,
                               const distance_matrix & dist) {
    // initialise vector with all edges
    edges = std::vector<simple_edge>(N*(N-1)/2);
    int pos = 0;
    for (vertex i=0; i<N; i++) {
        for (int j=i+1; j<N; j++) {
            edges[pos] = {i,j};
            pos++;
        }
    }

    // auto cmp = [&dist] (edge e1, edge e2) {
    //     return dist[e1.first][e1.second] <
    //     dist[e2.first][e2.second];
    // };

    // auto cmp2 = [&points] (edge e1, edge e2) {
    //     return square_distance(points[e1.first], points[e1.second]) <
    //     square_distance(points[e2.first], points[e2.second]);
    // };

    // auto cmp3 = [] (edge e1, edge e2) {
    //     return e1.d < e2.d;
    // };
    // auto cmp4 = [] (const void *e1, const void *e2) {
    //     kruskal_edge a = *(const kruskal_edge *)e1;
    //     kruskal_edge b = *(const kruskal_edge *)e2;
    //     return a < b;
    // };

    // std::sort(edges.begin(), edges.end());

                        
    // sort the edges
    std::sort(edges.begin(), edges.end(), 
              [&dist] (simple_edge e1, simple_edge e2) {
                  return dist[e1.first][e1.second] <
                      dist[e2.first][e2.second];
              });
}


void KruskalSolver::sort_edges(std::vector<small_edge> & id_edges) {
     std::sort(id_edges.begin(), id_edges.end());
}


/** 
 * trying to sort a smaller vector, for better caching
 */
void KruskalSolver::init_edges_v2(int N, std::vector<kruskal_edge> & edges,
                   const distance_matrix & dist) {
    // initialise vector with all edges
    int E = N*(N-1)/2;

    std::vector<kruskal_edge> edge_map(E);
    std::vector<small_edge> id_edges(E);
                           
    int pos = 0;
    for (vertex i=0; i<N; i++) {
        for (vertex j=i+1; j<N; j++) {
            edge_map[pos] = {i,j};
            id_edges[pos] = {pos,dist[i][j]};
            pos++;
        }        
    }
    sort_edges(id_edges);
    edges = std::vector<kruskal_edge>(E);
    for (int pos=0; pos<E; pos++) {
        int id = id_edges[pos].id;
        edges[pos] = edge_map[id];
    }
}

KruskalSolver::KruskalSolver(const distance_matrix & dist, int N, 
                             const std::vector<Point> & points,
                             // const adjacency_list & adjlist,
                             adjacency_list & mst,
                             int* root
                             ) :
    dist(dist), N(N), points(points), // adjlist(adjlist), 
    mst(mst), root(root),
    ds(N, mst), edges()
{}

void KruskalSolver::setup() {
    init_edges(N, edges, dist);
    // init_edges_v2(N, edges, dist);
}

void KruskalSolver::solve() {
    union_find(ds, edges);
    *root = ds.root();
}


///////////////////////////////////////////////////////////////////
////////// PrimSolver /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

    
PrimSolver::PrimSolver(const distance_matrix & dist, int N, 
                       const std::vector<Point> & points,
                       const adjacency_list & adjlist,
                       adjacency_list & mst,
                       int* root) :
    dist(dist), N(N), 
    points(points), adjlist(adjlist),
    mst(mst), root(root),

    visited(N, false),
    shortest_distance(N, (DIST) not_adjacent),
    cheapest_edge(N, not_adjacent),
    Q()
{}


void PrimSolver::setup() {
    *root = 0; // start at node 0, why not
    visited[0] = true;

    for ( vertex i=0; i<N; i++ ) {
        size_t num_adjacent = adjlist[i].size();
        mst[i].reserve(num_adjacent);
    }
    // prioq_lt cmp;
    for ( int neighbour : adjlist[*root] ) {
        cheapest_edge[neighbour] = *root;
        DIST d = dist[*root][neighbour];
        shortest_distance[neighbour] = d;
        Q.emplace(neighbour,d);
    }
}

void PrimSolver::solve() {
    DIST walk = 0;
    while (! Q.empty() ) {
        prioq_edge curr_edge = Q.top();
        vertex curr = curr_edge.first;
        Q.pop();
        // if the node already is visited, we have already expanded that node
        if ( visited[curr] ) continue;
        
        // We are adding edge that does not create a cycle
        visited[curr] = true;
        vertex whence = cheapest_edge[curr];
        // add edge whence->curr;
        mst[whence].push_back(curr);
        #ifdef CERR_MST
        std::cerr << whence << " " << curr << std::endl;
        #endif

        walk += dist[whence][curr];
        // Point ap = points[whence],
        //     bp = points[curr];
        // std::cerr << ap.x << "," << ap.y << ";" << bp.x << "," << bp.y << std::endl;
        expand_adjacent(curr);
    }
#ifdef CERR_MST
    std::cerr << "Root: " << *root << std::endl;
#endif
    std::cerr << "Size of MST (Prim): " << walk << std::endl;
}

void PrimSolver::expand_adjacent(vertex curr) {
    // update {adjacent}
    for ( vertex neighbour : adjlist[curr] ) {
        DIST d = dist[curr][neighbour];

        // first time we expand to that vertex
        if ( shortest_distance[neighbour] == not_adjacent ) {
            cheapest_edge[neighbour] = curr;
            shortest_distance[neighbour] = d;
            Q.emplace(neighbour, d);
        }

        // this node provides a shortcut
        else if (d < shortest_distance[neighbour] ) {
            cheapest_edge[neighbour] = curr;
            shortest_distance[neighbour] = d;
            Q.emplace(neighbour, d);
        }
    }

}
