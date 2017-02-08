//============================================================================
// Name        : node_info.cpp
// Author      : Sávio S. Dias
// E-mail      : diasssavio@gmail.com
// Institution : Universidade Federal Fluminense
// Description : Node Info class implementation
//============================================================================

#include "../../include/exact/node_info.h"

node_info::node_info(unsigned _depth) : depth(_depth), obj_value(0.0) {}

unsigned node_info::get_depth() const {
  return this->depth;
}

void node_info::init_root_data() {
  root_data = new node_info(0);
}

node_info* node_info::root_data = NULL;
