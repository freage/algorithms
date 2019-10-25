#ifndef MST_HEURISTIC_HPP
#define MST_HEURISTIC_HPP

#include "common.hpp"

/**
 * mst_heuristic.hpp -- The simple 2-apx heuristic based on MST
 *
 */

void mst_kruskal(int N, const distance_matrix & dist,
                 std::vector<int> & tour, const std::vector<Point> & points);

void mst_prim(int N, const distance_matrix & dist,
              std::vector<int> & tour, const std::vector<Point> & points);


#endif
