#include "dense_parallel.h"

#include <queue>
#include <cstdio>
#include <chrono>

static int thread_searched[MAX_THREADS];

std::vector<Node *> bfs(Node *src, Node *des)
{
  int thread_id = omp_get_thread_num();
  int thread_searched_local = ++thread_searched[thread_id];

  std::queue<Node *> q;
  std::vector<Node *> path;

  q.push(des); // because we need to return a src to des path
  while (!q.empty())
  {
    Node *cur = q.front();
    q.pop();

    if (cur == src)
    { 
      while (cur != des)
      {
        path.push_back(cur);
        cur = cur->prev[thread_id];
      }
      path.push_back(des);
      break;
    }
    for (Node *child : cur->children)
    {
      if (child->vis[thread_id] == thread_searched_local)
      continue;
      
      child->vis[thread_id] = thread_searched_local;
      child->prev[thread_id] = cur;
      q.push(child);
    }
  }
  return path;
}

void parallel_search(std::vector<Node *> paths[TASK_N])
{

  printf("Searching Paths...\n");
  printf("[>>>>>>>>>>>>>>>>>>>>>>>>>>>>]\n");
  auto begin_time = std::chrono::high_resolution_clock::now();

  int part = TASK_N / 30;
  int searched = 1;
  #pragma omp parallel for num_threads(MAX_THREADS)
  for(int i = 0; i < TASK_N; i++)
  {
    paths[i] = bfs(tasks[i][0], tasks[i][1]);

    int searched_local;
    #pragma omp atomic capture
    {
      searched_local = searched;
      searched++;
    }
    if (searched_local % part == 0)
    {
      printf(">");
      fflush(stdout);
    }
  }
  auto end_time = std::chrono::high_resolution_clock::now();
  printf("\nElasped : %.3lf s\n", 
  std::chrono::duration_cast<std::chrono::microseconds>(end_time - begin_time)
  .count() / 1000000.0);
}