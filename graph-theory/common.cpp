
#include <stdio.h>
#include <math.h>
#include <sys/time.h>

#include "common.hpp"

double start_time;

double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double)tv.tv_sec + ((float)tv.tv_usec) / 1000000;
}

double time_left() {
    return MAX_TIME - (get_time() - start_time);
}

DIST tour_length(int N, const distance_matrix & dist, std::vector<int> & tour) {
    // Also validate tour.
    std::vector<bool> are_points_present(N, false);
    DIST sum = 0;
    for (int i = 0; i < N - 1; i++) {
        sum += dist[tour[i]][tour[i+1]];
        are_points_present[tour[i]] = are_points_present[tour[i+1]] = true;
    }
    sum += dist[tour[0]][tour[N-1]];
    for (auto b: are_points_present) {
        assert(b);
    }
    return sum;
}

DIST distance(const Point & a, const Point & b) {
    double diffx = a.x - b.x;
    double diffy = a.y - b.y;
    double norm = sqrt(diffx*diffx + diffy*diffy);
#ifdef INT_DISTANCES
    return lrint(norm);
#else
    return norm;
#endif
}

void read_points(int N, std::vector<Point> & points) {
    for (int i = 0; i < N; i++) {
        scanf("%lf%lf", &points[i].x, &points[i].y);
    }
}

void get_distances(int N, const std::vector<Point> & points, distance_matrix & dist) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            dist[i][j] = distance(points[i], points[j]);
        }
    }
}

void read_input(int N, std::vector<Point> & points, distance_matrix & dist) {
    start_time = get_time();

    read_points(N, points);
    get_distances(N, points, dist);
}

void print_tour(int N, const std::vector<int> & tour) {
    for (int i = 0; i < N; i++) {
        printf("%d\n", tour[i]);
    }
}

void node_next(Node **cur, Node **prev) {
    if ((*cur)->a == *prev) {
        *prev = *cur;
        *cur = (*cur)->b;
    } else {
        *prev = *cur;
        *cur = (*cur)->a;
    }
}

Node **node_endpoint(Node & node) {
    if (node.a == NULL) {
        return &node.a;
    } else if (node.b == NULL) {
        return &node.b;
    } else {
        return NULL;
    }
}

// Point read_points() {
//     scanf("%d", &n);
//     printf("%d", n);
// }
