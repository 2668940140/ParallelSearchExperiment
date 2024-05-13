#include "dense_parallel.h"

std::vector<Node *> paths[TASK_N];

int main(int, char**){
  allocate_memory();
  generate_graph();
  generate_tasks();

  parallel_search(paths);

  return 0;
}
