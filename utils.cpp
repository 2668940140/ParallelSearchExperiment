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
  printf("Graph generated with %d nodes.\n", NODES_N);
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
  printf("%d tasks generated.\n", TASK_N);
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

bool check_tasks(std::vector<Node *> const paths[])
{
  bool ok = true;
  #pragma omp parallel for num_threads(MAX_THREADS)
  for (int i = 0; i < TASK_N; i++)
  {
    if (paths[i].size() == 0)
    {
      #pragma omp atomic write
      ok = false;
    }
    if (paths[i].front() != tasks[i][0] ||
        paths[i].back() != tasks[i][1])
    {
      #pragma omp atomic write
      ok = false;
    }
    for(int j = 0; j < (int)paths[i].size() - 1; j++)
    {
      Node *node = paths[i][j], *node_next = paths[i][j + 1];
      bool found = false;
      for(auto child : node->children)
      {
        if (child == node_next)
        {
          found = true;
          break;
        }
      }
      if (!found)
      {
        #pragma omp atomic write
        ok = false;
      }
    }
  }

  if (ok)
    printf("Accepted!\n");
  else
    printf("Wrong Answer\n");
  return ok;
}