#include "djikstra.h"

bool all_visited (bool *visited, int v)
{
  for (int i = 0; i < v; ++i) {
    if (visited[i] == 0) { return false; }
  }
  return true;
}

int index_of_min_unvisited (int **adj, bool *visited, int current_node, int v)
{
  int min = INT_MAX, min_i = INT_MAX;
  for (int j = 0; j < v; ++j) {
    if ( adj[current_node][j] != 0 && !visited[j] && min > adj[current_node][j] ) {
      min = adj[current_node][j];
      min_i = j;
    }
  }
  return min_i;
}

// TODO: This seg faults when start node = 1 for map A
int *djikstra_Ov2 (int **adj, int v, int start_node)
{
  bool *visited = (bool *)calloc(v, sizeof(*visited));
  int *distances = (int *)calloc(v, sizeof(*distances));
  int current_node = start_node;
  int orig, new;
  for (int i = 0; i < v; ++i) {
    distances[i] = ( (i == start_node) ? 0 : INT_MAX );
  }
  do {
    for (int j = 0; j < v; ++j) {
      if ( adj[current_node][j] != 0 && !visited[j] ) {
	orig = distances[j];
	new = distances[current_node] + adj[current_node][j];
	distances[j] = ( (orig > new) ? new : orig );
      }
    }
    visited[current_node] = 1;
    current_node = index_of_min_unvisited(adj, visited, current_node, v);
  } while ( !all_visited(visited, v) );

  free(visited);
  return distances;
}

/* A function that takes a (<type> **) as a param can take a (<type> *[<size>]) but not a (<type> [<size>][<size>])
int main ()
{
  int v = 0;
  //  int a0[6] = {0,  7,  0,  9, 0, 0};
  //  int a1[6] = {7,  0,  2, 10, 0, 0};
  //  int a2[6] = {0,  2,  0, 15, 0, 0};
  //  int a3[6] = {9, 10, 15,  0, 5, 7};
  //  int a4[6] = {0,  0,  0,  5, 0, 4};
  //  int a5[6] = {0,  0,  0,  7, 4, 0};
  int a6[0] = {};
  int *a[6] = {a6};//{a0, a1, a2, a3, a4, a5};

  int *d = (int *)calloc(v, sizeof(*d));
  d = djikstra_Ov2(a, v, 0);
  for (int i = 0; i < v; ++i) {
    printf( (i != v-1) ? "%d " : "%d\n", d[i] );
  }
  free(d);
  return 0;
}
*/
