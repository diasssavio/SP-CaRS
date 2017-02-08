//============================================================================
// Name        : mincut.h
// Author      : Sávio S. Dias
// E-mail      : diasssavio@gmail.com
// Institution : Universidade Federal Fluminense
// Description : Minimum Cut class interface
//============================================================================

#ifndef SRC_MINCUT_H_
#define SRC_MINCUT_H_

#include <lemon/smart_graph.h>
#include <lemon/hao_orlin.h>
#include <lemon/edmonds_karp.h>

#include <vector>
#include <set>
#include <iostream>
#include <fstream>

#include "typedef.hpp"

using namespace std;
using namespace lemon;

class mincut {
private:
  unsigned N;
  SmartDigraph *graph;
  SmartDigraph::ArcMap<double> *dist;
  SmartDigraph::NodeMap<bool> *cut;

  HaoOrlin<SmartDigraph, SmartDigraph::ArcMap<double>, Tolerance<double> > *alg;

  void init();
  void build_digraph(unsigned, vector<set<unsigned> >&, vector<vector<double> >&);

public:
  mincut();
  virtual ~mincut();

  unsigned add_node();
  unsigned add_arc(unsigned, unsigned);
  void add_arc_cost(unsigned, double);

  double generate_min_cut();
  void run(unsigned, vector<set<unsigned> >&, vector<vector<double> >&);

  bool is_node_in_cut(unsigned);
  void print_graph(ofstream&);

};

#endif /* SRC_MINCUT_H_ */
