/**
 * It is true that there are other more efficient algorithms for 
 * this multiple shortest path problem. It is just used as an
 * example using parallel searching.
*/

#ifndef DENSE_PARALLEL_H
#define DENSE_PARALLEL_H
#include "utils.h"

#include <cassert>
#include <vector>

extern int nodes_pushed;
extern int nodes_poped;


/// @brief This function performs a parallel breadth-first search on a graph.
/// @returns A vector of nodes that represent the path from the source to the destination.
/// A empty vector if no path found.
std::vector<Node *> bfs(Node *src, Node *des);

void parallel_search(std::vector<Node *> paths[TASK_N]);

#endif