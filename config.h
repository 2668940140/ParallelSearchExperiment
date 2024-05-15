#ifndef CONFIG_H
#define CONFIG_H

static constexpr int MAX_THREADS = 32;

/* The number of nodes of one a graph*/
static constexpr int NODES_N = 1e6;

/* The minimum initial number of edges of one node*/
static constexpr int MIN_EDGE_N = 1;

/* The maximum initial number of edges of one node*/
static constexpr int MAX_EDGE_N = 10;

static constexpr int TASK_N = 1e3;

static constexpr int SEED = 0;

/* If true, the generated graph and tasks is not deterministic with seed*/
static constexpr bool PARALLEL_GENERATE = false;

/* This is not exact beacuse of struct alignment*/
static constexpr int EXTRA_NODE_SIZE = 2048;

/* If true, program will write EXTRA_NODE_SIZE bytes when visiting a node*/
static constexpr int WRITE_TO_EXTRA = true;

/** If true, program will record the number of pushed and poped nodes
 * This is useful for debugging and performance analysis, but it is slow.
*/
static constexpr bool RECODE_NODES_PUSHED_POPED = false;

#endif