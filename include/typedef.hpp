#ifndef TYPEDEF_HPP_
#define TYPEDEF_HPP_

// SYSTEM LIBRARIES
#include <ilcplex/ilocplex.h>
#include <ilcplex/ilocplexi.h>
#include <vector>

#define MAX_DOUBLE 1e+75

#define EPSILON 1e-6

///// CONCERT
typedef IloArray<IloNumVarArray> IloNumVarArray2;
typedef IloArray<IloNumVarArray2> IloNumVarArray3;
typedef IloArray<IloNumVarArray3> IloNumVarArray4;
typedef IloArray<IloNumVarArray4> IloNumVarArray5;

typedef IloArray<IloNumArray2> IloNumArray3;
typedef IloArray<IloNumArray3> IloNumArray4;

typedef IloArray<IloFloatVarArray> IloFloatVarArray2;
typedef IloArray<IloFloatVarArray2> IloFloatVarArray3;
typedef IloArray<IloFloatVarArray3> IloFloatVarArray4;

typedef IloArray<IloBoolVarArray> IloBoolVarArray2;
typedef IloArray<IloBoolVarArray2> IloBoolVarArray3;
typedef IloArray<IloBoolVarArray3> IloBoolVarArray4;

//typedef IloArray<IntegerFeasibilityStatus> IntegerFeasibilityArray;
//typedef IloArray<ControlCallbackl::IntegerFeasibilityArray> IntegerFeasibilityArray2;

typedef std::vector<int>::iterator VecIntIT;
typedef std::vector<int>::const_iterator VecIntCIT;
typedef std::vector<int> VecInt;

typedef struct edge {
  int v1, v2;
} str_edge;

typedef struct cut {
  double viol;
  std::vector<int> corte;
  std::vector<int> S;
} str_cut;

typedef struct Node {
  int id;
  struct Arc * first_arc; // Cyclic list of incident edges
  struct Arc * scan_ptr; // Next arc to be visited when node will be visited again

  // Entries for use by gmindcut
  int dist;
  double excess;
  struct Node * bfs_link; // One way BFS working queue
  struct Node * stack_link; // Stack of active node
  struct Node * left_link; // Doubly linked lists
  struct Node * right_link; // Dormant and W nodes
  bool unmarked; // For BFS in progress
  bool in_S; // Set of source nodes
  bool in_W; // Set of W-valid nodes
  bool shore1; // Final mark for placement of node in one of the cut shores

  bool terminal; // Is the node a terminal one?
} node;

typedef struct Arc {
  node * adjac; // Head node of arc
  struct Arc * next; // Incident list of tail node
  struct Arc * back; // Pointer to reverse arc
  double cap; // Arc capacity
  double rcap; // Arc residual capacity used in gmindcut

  int index;
} arc;

typedef struct Graph {
  int n_nodes;
  std::vector<node> nodes;
  int n_arcs;
  std::vector<arc> arcs;
  int n_terminals;
  node * Wptr;
  int raiz;
  int sink;
} graph;

#endif /* TYPEDEF_HPP_ */
