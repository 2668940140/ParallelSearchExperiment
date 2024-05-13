#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include "config.h"

#include <vector>
#include <cstring>
#include <omp.h>

template <int extra_node_size>
struct _Node
{
  std::vector<_Node *> children;
  _Node *prev[MAX_THREADS];
  int vis[MAX_THREADS];
  omp_lock_t lock;

  char extra_data[extra_node_size];

  _Node()
  {
    omp_init_lock(&lock);
    memset(vis, -1, sizeof(vis));
    
  }
  ~_Node()
  {
    omp_destroy_lock(&lock);
  }
};

template<>
struct _Node<0>
{
  std::vector<_Node *> children;
  _Node *prev[MAX_THREADS];
  int vis[MAX_THREADS];
  omp_lock_t lock;

  _Node()
  {
    omp_init_lock(&lock);
    memset(vis, -1, sizeof(vis));
    
  }
  ~_Node()
  {
    omp_destroy_lock(&lock);
  }
};

typedef _Node<EXTRA_NODE_SIZE> Node;

#endif