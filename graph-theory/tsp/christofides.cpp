#include <cassert>
#include <functional>
#include <iostream>

#include "christofides.hpp"
#include "mst.hpp"


/** christofides.cpp -- A solver using Christofides' algorithm
 *
 * Created by Freddie on Fri Oct 28 18:43:07 2016
 */



///////////////////////////////////////////////////////////
///////////// Public API //////////////////////////////////
///////////////////////////////////////////////////////////

void christofides_prim(int N, const distance_matrix & dist,
                       std::vector<vertex> & tour,
                       const std::vector<Point> & points) {
    Christofides c(N, dist, tour, points);
    c.run_prim();
    c.find_matching();
    c.find_euler_tour();
}

void christofides_kruskal(int N, const distance_matrix & dist,
                          std::vector<vertex> & tour,
                          const std::vector<Point> & points) {
    Christofides c(N, dist, tour, points);
    c.run_kruskal();
    c.find_matching();
    c.find_euler_tour();
}


///////////////////////////////////////////////////////////
/////////// Class public functions ////////////////////////
// Note: state dependent functions                       //
///////////////////////////////////////////////////////////


Christofides::Christofides(int N, const distance_matrix & dist,
                           std::vector<vertex> & tour,
                           const std::vector<Point> & points) :
    N(N), dist(dist), tour(tour), points(points),
    root(), mst(N, std::vector<vertex>()),
    adjlist(N, std::vector<vertex>() ),
    odds(),
    tour_adjlist(N, std::vector<tour_edge*>())
{ }

void Christofides::run_prim() {
    // if usual metric heuristics, all nodes are adjacent
    for (vertex i=0; i<N; i++) {
        adjlist[i].reserve(N-1);
        for (int j=0; j<N; j++) {
            if (i==j) continue; // a point is not adjacent to itself
            adjlist[i].push_back(j);
        }
    }
    // std::cerr << "Beginning MST (Prim): " << time_left() << std::endl;
    // creates a directed MST
    prim(dist, N, points, adjlist, mst, &root);
    // std::cerr << "Done MST (Prim): " << time_left() << std::endl;
}

void Christofides::run_kruskal() { 
    // if usual metric heuristics, all nodes are adjacent
    // no support for custom adjacency list yet

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
    // std::cerr << "Beginning MST (Kruskal): " << time_left() << std::endl;
    kruskal(dist, N, points, mst, // adjlist,
            &root);
    // std::cerr << "Done MST (Kruskal): " << time_left() << std::endl;
}

void Christofides::find_matching() {
    std::vector<vertex> odds;
    find_odds(N, mst, odds, root);
    // std::cerr << "Found odd vertices: " << time_left() << std::endl;

    // Now MST+matching will be transferred to a `tour_adjacency_list`
    // (a bidirectional adjacency list)
    tour_adjlist = tour_adjacency_list(N, std::vector<tour_edge*>());
    size_t num_odds = odds.size();
    size_t num_tour_edges = num_odds + 2*(N-1);

    // create matching -- puts matching in tour_adjlist
    // if (num_odds < 100) {
    //     greedy_matching_x2(N, dist, odds, tour_adjlist);
    // } else {
        greedy_matching_v2(N, dist, odds, tour_adjlist);
    // }
    // std::cerr << "Matching done: " << time_left() << std::endl;
    // complete the tour_adjacency_list:
    for ( vertex i=0; i<N; i++ ) {
        for ( vertex j : mst[i] ) {
            tour_edge* e = new tour_edge(i,j,false);
            tour_adjlist[i].push_back(e);
            tour_adjlist[j].push_back(e);
        }
    }
    size_t debug_sum = 0;
    for ( vertex i=0; i<N; i++ ) {
        debug_sum += tour_adjlist[i].size();
    }
    assert( debug_sum == num_tour_edges );

    // std::cerr << "Adjacency list created: " << time_left() << std::endl;
}

void Christofides::find_euler_tour() {
    // std::cerr << "Time: Adjacency list created: " << time_left() << std::endl;
    tour_segment etour; // euler_tour
    
    euler_tour(tour_adjlist, etour, root);
    // std::cerr << "Time: Euler tour complete: " << time_left() << std::endl;
    
    tour.clear();
    tour.reserve(N);
    std::vector<vertex> tour_v2 = tour;
    shortcutting(N, etour, tour, root);
    shortcutting_reverse(N, etour, tour_v2, root);
    // std::cerr << "Time: Shortcutting complete: " << time_left() << std::endl;

    DIST len = tour_length(N, dist, tour);
    DIST reverse_len = tour_length(N, dist, tour_v2);

    // std::cerr << len << " " << reverse_len << std::endl;
    
    if (reverse_len < len ) {
        tour = tour_v2;
    }

    // free memory
    for ( vertex i=0; i<N; i++ ) {
        for (tour_edge* e : tour_adjlist[i] ) {
            // delete only second time we see this edge
            if (i == std::max(e->fst, e->snd)) 
                delete e;
        }
    }
    // std::cerr << "Memory freed: " << time_left() << std::endl;
}

///////////////////////////////////////////////////////////
////////////////// Class private functions ////////////////
///////////////////////////////////////////////////////////


void Christofides::find_odds(int N, const adjacency_list & mst,
                             std::vector<vertex> & odds, vertex root) {
    assert( odds.size() == 0 );
    for ( vertex i=0; i<N; i++ ) {
        if (i == root) {
            // special case: compare below
            // root does not have a parent
            if (mst[i].size() % 2 == 1) {
                odds.push_back(i);
            }
        }
        // i is adjacent to mst[i] + parent
        // -- i.e. an odd number if mst[i].size() is even
        else if ( mst[i].size() % 2 == 0 ) { // assumes directed mst
            odds.push_back(i);
        }
    }
}

/**
 * do a greedy matching and add edges to the tour_adjacency_list
 */
void Christofides::greedy_matching(int N, const distance_matrix & dist,
                                   const std::vector<vertex> & odds,
                                   tour_adjacency_list & matching,
                                   DIST*weight)
{
    //assert( matching.size() == 0 );

    if (weight != nullptr) {
        *weight = 0;
    }
    
    size_t num_odds = odds.size();
    assert(num_odds % 2 == 0);
    std::vector<bool> matched(N, false);

    const DIST infty = 10e8;
    for (vertex v : odds) {
        if (matched[v]) continue;

        matched[v] = true;

        DIST length = infty;
        int match = v; // just some init val
        for (vertex u : odds) {
            if (matched[u]) continue;

            DIST d = dist[v][u];
            if (d < length) {
                match = u;
                length = d;
            }
        }
        matched[match] = true;
        assert( match != v );
        if (weight != nullptr){
            *weight += length;
        }
        tour_edge* e = new tour_edge(match,v,false);
        matching[v].push_back(e);
        matching[match].push_back(e);
    }
}


/**
 * do a greedy matching and add edges to the tour_adjacency_list
 */
void Christofides::greedy_matching_v2(int N, const distance_matrix & dist,
                                      const std::vector<vertex> & odds,
                                      tour_adjacency_list & matching,
                                      DIST*weight)
{

    if (weight != nullptr) {
        *weight = 0;
    }

    size_t num_odds = odds.size();
    assert(num_odds % 2 == 0);

    std::vector<tour_edge*> candidates;
    candidates.reserve(num_odds*(num_odds-1)/2);
    
    // for (auto v = odds.begin(); v != odds.end(); ++v ) {
    //     for (auto u = v+1; u != odds.end(); ++u ) {
    for (vertex v : odds) {
        for (vertex u : odds ) {
            if (v > u) {
                candidates.push_back(new tour_edge(u,v,false));
            }
        }
    }
    std::sort(candidates.begin(), candidates.end(),
              [&dist] (tour_edge* e1, tour_edge* e2) -> bool {
                  return dist[e1->fst][e1->snd] < dist[e2->fst][e2->snd];
              });

    std::vector<bool> matched(N, false);
    
    for (tour_edge* edge : candidates) {
        if (matched[edge->fst] || matched[edge->snd] ) {
            delete edge;
        }
        else {
            matched[edge->fst] = true;
            matched[edge->snd] = true;
            matching[edge->fst].push_back(edge);
            matching[edge->snd].push_back(edge);
            if (weight != nullptr){
                *weight += dist[edge->fst][edge->snd];
            }

        }
    }
}

void Christofides::greedy_matching_x2(int N, const distance_matrix & dist,
                                   const std::vector<vertex> & odds,
                                   tour_adjacency_list & matching)
{
    tour_adjacency_list loop_match(N);
    tour_adjacency_list sort_match(N);
    DIST loop_weight, sort_weight;
    greedy_matching(N, dist, odds, loop_match, &loop_weight);
    greedy_matching_v2(N, dist, odds, sort_match, &sort_weight);
    std::cerr << loop_weight << " " << sort_weight << std::endl;
    std::vector<bool> matched(N, false);
    tour_adjacency_list* best;
    // let best be an alias without copying the whole thing...
    if (sort_weight < loop_weight) {
        best = &sort_match;
    }
    else {
        best = &loop_match;
    }
    for (auto row : *best ) {
        for (tour_edge* edge : row ) {
            if (!matched[edge->fst]) {
                matching[edge->fst].push_back(edge);
                matching[edge->snd].push_back(edge);
                matched[edge->fst] = true;
                matched[edge->snd] = true;
            }
        }
    }
}


void Christofides::euler_tour(const tour_adjacency_list & adjlist,
                              tour_segment & result,
                              vertex root)
{
    assert( result.empty() ); 

    typedef tour_segment::iterator tour_iter;
    
    std::function<void(vertex, tour_iter)> dfs =
        [&dfs, &adjlist, &result] 
        (vertex curr, tour_iter result_position) -> void {
        for ( tour_edge* e : adjlist[curr] ) {
            if ( e->visited ) continue;
            e->visited = true;

            tour_iter new_position = [&result_position,
                                      &result,
                                      &e] () -> tour_iter {
                if (result_position == result.end()) {
                    result.push_front(e);
                    return result.begin();
                } else {
                    tour_iter one_step_ahead = ++result_position;
                    --result_position;
                    // insert for list is `insert_before`
                    return result.insert(one_step_ahead, e);
                }
            }();
            
            dfs(e->follow(curr), new_position);
        }
    };
    dfs(root, result.begin());
}


double Christofides::absval(Point p) {
    return sqrt(p.x*(p.x) + p.y*(p.y));
}

/**
 * scalar product of `with` and delta:=(`to` -`from`), divided by abs(delta).
 * So this gives the projection of `with` on `delta`.
 */
double Christofides::scalarprod(const Point & with, const Point & from, const Point & to) {
    Point delta;
    delta.x = to.x - from.x;
    delta.y = to.y - from.y;
    return (with.y * (delta.y) + with.x * (delta.x)) / absval(delta);
}


void Christofides::order_children(int N, int parent, int current,
                                  tour_adjacency_list & adjlist,
                                  const std::vector<Point> & points)
{
    // one edge we came from and one out - trivial
    if (adjlist[current].size() < 3) return;
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
    for (tour_edge* e : adjlist[current]) {
        vertex c = e->follow(current);
        if ( e->visited ) {
            // since we will just skip them anyway
            compare[c] = 0;
            continue;
        }
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
              [&compare, &current] (tour_edge* to_a, tour_edge* to_b) -> bool {
                  vertex a = to_a->follow(current);
                  vertex b = to_b->follow(current);
                  return compare[a] < compare[b];
              });
}

void Christofides::euler_tour_v2(tour_adjacency_list & adjlist,
                                 tour_segment & result,
                                 vertex root)
{
    assert( result.empty() ); 

    typedef tour_segment::iterator tour_iter;

    std::function<void(vertex, vertex, tour_iter)> dfs =
        [&dfs, &adjlist, &result, this] 
        (vertex curr, vertex parent, tour_iter result_position) -> void {

        order_children(N, parent, curr, adjlist, this->points);
        
        for ( tour_edge* e : adjlist[curr] ) {
            if ( e->visited ) continue;
            e->visited = true;

            tour_iter new_position = [&result_position,
                                      &result,
                                      &e] () -> tour_iter {
                if (result_position == result.end()) {
                    result.push_front(e);
                    return result.begin();
                } else {
                    tour_iter one_step_ahead = ++result_position;
                    --result_position;
                    // insert for list is `insert_before`
                    return result.insert(one_step_ahead, e);
                }
            }();
            
            dfs(e->follow(curr), curr, new_position);
        }
    };
    dfs(root, -1, result.begin());
}

void Christofides::shortcutting(size_t N, const tour_segment & etour,
                                std::vector<vertex> & tour,
                                vertex root)
{
    typedef tour_segment::const_iterator tour_iter;
    std::vector<bool> visited(N, false);
    vertex curr = root;
    for (tour_iter it=etour.begin(); it != etour.end(); ++it) {
        if (!visited[curr]) {
            tour.push_back(curr);
            visited[curr] = true;
        }
        curr = (*it)->follow(curr);
    }
    assert( tour.size() == N );
}

void Christofides::shortcutting_reverse(size_t N, const tour_segment & etour,
                                        std::vector<vertex> & tour,
                                        vertex root)
{
    typedef tour_segment::const_reverse_iterator tour_iter;
    std::vector<bool> visited(N, false);
    vertex curr = root;
    for (tour_iter it=etour.rbegin(); it != etour.rend(); ++it) {
        if (!visited[curr]) {
            tour.push_back(curr);
            visited[curr] = true;
        }
        curr = (*it)->follow(curr);
    }
    assert( tour.size() == N );
}
