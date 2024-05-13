#include "utils.h"

#include <random>
#include <set>
#include <sys/resource.h>

Node *graph;
Node *(*tasks)[2];


void generate_graph()
{
  std::uniform_int_distribution<> dis(MIN_EDGE_N, MAX_EDGE_N);
  printf("Generating Graph...\n");
  printf("[>>>>>>>>>>>>>>>>>>>>>>>>>>>>]\n");
  int part = NODES_N / 30;
  
  if (PARALLEL_GENERATE)
  {
    std::random_device rd;
    int generated = 1;

    #pragma omp parallel for num_threads(MAX_THREADS)
    for(int i = 1; i < NODES_N; i++) {
      std::minstd_rand gen(rd());
      int edge_n = dis(gen);
      std::set<int> children_set;

      std::uniform_int_distribution<> dis2(0, i - 1);
      for(int j = 0; j < edge_n; j++) {
        int child = dis2(gen);
        children_set.insert(child);
      }

      omp_set_lock(&graph[i].lock);
      for(auto child : children_set)
      {
        omp_set_lock(&graph[child].lock);
        graph[child].children.push_back(&graph[i]);
        omp_unset_lock(&graph[child].lock);
        graph[i].children.push_back(&graph[child]);
      }
      omp_unset_lock(&graph[i].lock);

      int generated_local;
      #pragma omp atomic capture
      {
        generated_local = generated;
        generated++;
      }

      if (generated_local % part == 0)
      {
        printf(">");
        fflush(stdout);
      }
    }
  }
  else
  {
    std::minstd_rand gen(SEED);
    for(int i = 1; i < NODES_N; i++) {
      int edge_n = dis(gen);
      std::set<int> children_set;

      std::uniform_int_distribution<> dis2(0, i - 1);
      for(int j = 0; j < edge_n; j++) {
        int child = dis2(gen);
        children_set.insert(child);
      }

      for(auto child : children_set)
      {
        graph[child].children.push_back(&graph[i]);
        graph[i].children.push_back(&graph[child]);
      }

      if (i % part == 0)
      {
        printf(">");
        fflush(stdout);
      }
    }
  }
  printf("\n");
}

void generate_tasks()
{
  std::random_device rd;
  std::uniform_int_distribution<> dis(0, NODES_N - 1);

  int generated = 1;

  printf("Generating Tasks...\n");
  printf("[>>>>>>>>>>>>>>>>>>>>>>>>>>>>]\n");
  int part = TASK_N / 30;
  if (PARALLEL_GENERATE)
  {
  #pragma omp parallel for num_threads(MAX_THREADS)
  for(int i = 0; i < TASK_N; i++)
  {
    int src = dis(rd);
    int dst = src;
    while (dst == src)
    {
      dst = dis(rd);
    }

    tasks[i][0] = &graph[src];
    tasks[i][1] = &graph[dst];

    int generated_local;
    #pragma omp atomic capture
    {
      generated_local = generated;
      generated++;
    }
  
    if (generated_local % part == 0)
    {
      printf(">");
      fflush(stdout);
    }
  }
  }
  else
  {
    for(int i = 0; i < TASK_N; i++)
    {
      int src = dis(rd);
      int dst = src;
      while (dst == src)
      {
        dst = dis(rd);
      }

      tasks[i][0] = &graph[src];
      tasks[i][1] = &graph[dst];

      if (generated % part == 0)
      {
        printf(">");
        fflush(stdout);
      }
      generated++;
    }
  }
  printf("\n");
}

long getMemoryUsage() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss;
}
void allocate_memory()
{
  long before_size = getMemoryUsage();
  printf("Allocating Memory...\n");
  graph = new Node[NODES_N];
  printf("Graph Mem Size : %zu Mb\n", (getMemoryUsage() - before_size) / 1024);
  tasks = new Node *[TASK_N][2];
  printf("Allocated.\n");
}