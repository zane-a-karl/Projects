#include <stdio.h>
#include <limits.h> // INT_MAX
#include <stdbool.h>
#include <stdlib.h>


#ifndef _DJIKSTRA_H_
#define _DJIKSTRA_H_

bool all_visited (bool *visited, int v);

int index_of_min_unvisited (int **adj, bool *visited, int current_node, int v);

int *djikstra_Ov2 (int **adj, int v, int start_node);


#endif // _DJIKSTRA_H_
