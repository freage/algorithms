#include "common.hpp"
#include "mst.hpp"
#include <cmath>
#include <functional>

/** mst_heuristic.cpp -- The simple 2-apx heuristic based on MST
 *
 */

double absval(Point p) {
    return sqrt(p.x*(p.x) + p.y*(p.y));
}

/**
 * scalar product of `with` and delta:=(`to` -`from`), divided by abs(delta).
 * So this gives the projection of `with` on `delta`.
 */
double scalarprod(const Point & with, const Point & from, const Point & to) {
    Point delta;
    delta.x = to.x - from.x;
    delta.y = to.y - from.y;
    return (with.y * (delta.y) + with.x * (delta.x)) / absval(delta);
}


void order_children(int N, int parent, int current,
                    adjacency_list & adjlist,
                    const std::vector<Point> & points)
{    
    if (adjlist[current].size() < 2) return;
    Point par, 
        curr = points[current];
    if (parent == -1) { // the root
        // create a dummy parent
        par.x = curr.x - 2;
        par.y = curr.y;        
    } else {
        par = points[parent];
    }

    // a vector from curr to par
    Point dpar;
    // a vector orthogonal to dpar (rotation 90 deg clock-wise)
    Point norm;
    norm.y = dpar.x = par.x - curr.x;
    dpar.y = par.y - curr.y;
    norm.x = -dpar.y;
    
    std::vector<double> compare(N);
    double P = absval(dpar);
    for (int c : adjlist[current]) {
        Point child = points[c];
        // scalar product with the norm
        double with_norm = scalarprod(norm, curr, child);
        // scalar product with curr->parent
        double with_dpar = scalarprod(dpar, curr, child);
        // 3P >= value >= -P
        double value = (with_norm>0 ? -with_dpar : 2*P + with_dpar);
        compare[c] = value;
    }    

    std::sort(adjlist[current].begin(), adjlist[current].end(),
              /**
               * a < b ? (as angles from curr)
               */
              [&compare] (int a, int b) -> bool {
                  return compare[a] < compare[b];
              });
}

void preorder(int N,
              const std::vector<Point> & points,
              int root, adjacency_list & adjlist,
              std::vector<int> & tour) 
{
    int parent = -1;
    
    // todo: change to bfs
    std::function<void(int, int)> dfs =
        [&dfs, &adjlist, &points, &tour, &N] (int parent, int curr) -> void {
        int prev = parent;
        if (prev != -1) {
            prev = tour.back();
        }
        
        order_children(N, prev, curr, adjlist, points);
        tour.push_back(curr);        
        for (int child : adjlist[curr]) {
            dfs(curr, child);
        }
    };
    dfs(parent, root);
}

void mst_kruskal(int N, const distance_matrix & dist,
                 std::vector<int> & tour, const std::vector<Point> & points) {
    adjacency_list mst;
    int root;
    // // if 2-apx-heuristic, all nodes are adjacent
    // adjacency_list adjlist(N, std::vector<int>());
    // for (int i=0; i<N; i++) {
    //     adjlist[i].reserve(N-1);
    //     for (int j=0; j<N; j++) {
    //         if (i==j) continue; // a point is not adjacent to itself
    //         adjlist[i].push_back(j);
    //     }
    // }

    kruskal(dist, N, points, mst, // adjlist,
            &root);
    tour.clear();
    preorder(N, points, root, mst, tour);
}


void mst_prim(int N, const distance_matrix & dist, 
              std::vector<int> & tour, const std::vector<Point> & points) {
    int root;
    // store MST as an adjlist
    adjacency_list mst(N, std::vector<int>() );

    // if 2-apx-heuristic, all nodes are adjacent
    adjacency_list adjlist(N, std::vector<int>());
    for (int i=0; i<N; i++) {
        adjlist[i].reserve(N-1);
        for (int j=0; j<N; j++) {
            if (i==j) continue; // a point is not adjacent to itself
            adjlist[i].push_back(j);
        }
    }
    
    prim(dist, N, points, adjlist, mst, &root);

    tour.clear(); 
    preorder(N, points, root, mst, tour);
    
}
