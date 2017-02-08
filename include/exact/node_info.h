//============================================================================
// Name        : node_info.h
// Author      : Sávio S. Dias
// E-mail      : diasssavio@gmail.com
// Institution : Universidade Federal Fluminense
// Description : Node Info class interface
//============================================================================

#ifndef SRC_NODE_INFO_H_
#define SRC_NODE_INFO_H_

#include <ilcplex/ilocplex.h>

// Subclass storing data related to the current branching node
class node_info : public IloCplex::MIPCallbackI::NodeData {
private:
  unsigned const depth; // Node depth
  double obj_value; // Objective function value at the current node

public:
  node_info(unsigned);

  unsigned get_depth() const;

  static node_info* root_data;
  static void init_root_data();

};

#endif /* SRC_NODE_INFO_H_ */
