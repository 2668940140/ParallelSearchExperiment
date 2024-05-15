#include "dense_parallel.h"
#include <cstdio>

std::vector<Node *> paths[TASK_N];

int main(int, char**){
  printf("Hello Dense-Parallel :)\n");
  allocate_memory();
  generate_graph();
  generate_tasks();

  parallel_search(paths);
  assert(check_tasks(paths));

  return 0;
}
