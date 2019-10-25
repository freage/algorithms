#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <cassert>

#define MAX_TIME 2.0

struct TimeForOptSteps {
  static constexpr float TimeForImproveFragments  = 0.5f; //
  static constexpr float TimeForKOpt = 1.2f;
  static constexpr float TimeForSimulatedAnnealing  = 0.5f; //
};

#define TRUE 1
#define FALSE 0

#define INT_DISTANCES

#ifdef INT_DISTANCES
#define DFMT "%ld"
typedef long int DIST;
#else
#define DFMT "%lf"
typedef double DIST;
#endif

typedef int vertex;
typedef int point_index;
typedef int tour_index;

typedef std::vector<std::vector<DIST> > distance_matrix;
typedef std::vector<std::vector<vertex> > adjacency_list;

typedef struct {
    double x;
    double y;
} Point;

typedef struct node {
    int id;
    struct node *a;
    struct node *b;
    int aux;
} Node;

void read_input(int N, std::vector<Point> & points, distance_matrix & dist );
double time_left();
void print_tour(int N, const std::vector<int> & tour);
DIST tour_length(int N, const distance_matrix & dist, std::vector<int> & tour);
void node_next(Node **cur, Node **prev);
Node **node_endpoint(Node & node);

#endif
