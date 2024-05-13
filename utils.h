#ifndef UTILS_H
#define UTILS_H

#include "data_structure.h"

extern Node *graph;

extern Node *(*tasks)[2];

void allocate_memory();
void generate_graph();
void generate_tasks();

#endif