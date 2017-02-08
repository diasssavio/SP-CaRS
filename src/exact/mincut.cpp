//============================================================================
// Name        : mincut.cpp
// Author      : Sávio S. Dias
// E-mail      : diasssavio@gmail.com
// Institution : Universidade Federal Fluminense
// Description : Minimum Cut class implementation
//============================================================================

#include "../include/mincut.h"

mincut::mincut() {
  graph = NULL;
  alg = NULL;
  dist = NULL;
  cut = NULL;
  init();
}

mincut::~mincut() {
  delete graph;
  delete alg;
  delete dist;
  delete cut;
}

void mincut::init() {
  if(graph) delete graph;
  if(alg) delete alg;
  if(dist) delete dist;
  N = 0;
  graph = new SmartDigraph();
  dist = new SmartDigraph::ArcMap<double>(*graph);
}

unsigned mincut::add_node() {
  SmartDigraph::Node node = graph->addNode();
  N++;
  return graph->id(node);
}

unsigned mincut::add_arc(unsigned i, unsigned j) {
  SmartDigraph::Node node_i = graph->nodeFromId(i);
  SmartDigraph::Node node_j = graph->nodeFromId(j);
  SmartDigraph::Arc arc = graph->addArc(node_i, node_j);
  return graph->id(arc);
}

void mincut::add_arc_cost(unsigned id, double _cost) {
  SmartDigraph::Arc arc = graph->arcFromId(id);
  (*dist)[arc] = _cost;
}

void mincut::build_digraph(unsigned _N, vector<set<unsigned> >& neighbors, vector<vector<double> >& weights) {
  for(unsigned i = 0; i < _N; i++)
    add_node();

  for(unsigned i = 0; i < _N; i++)
    for(set<unsigned>::iterator j = neighbors[i].begin(); j != neighbors[i].end(); j++) {
      // if(i < *j) {
        unsigned id = add_arc(i, *j);
        (*dist)[ graph->arcFromId(id) ] = weights[i][*j];
      }
}

void mincut::run(unsigned _N, vector<set<unsigned> >& neighbors, vector<vector<double> >& weights) {
  build_digraph(_N, neighbors, weights);
  alg = new HaoOrlin<SmartDigraph, SmartDigraph::ArcMap<double>, Tolerance<double> >(*graph, *dist, Tolerance<double>(EPSILON));
  alg->run();
}

double mincut::generate_min_cut() {
  cut = new SmartDigraph::NodeMap<bool>(*graph);
  return alg->minCutMap(*cut);
}

bool mincut::is_node_in_cut(unsigned id) {
  return (*cut)[ graph->nodeFromId(id) ];
}

void mincut::print_graph(ofstream& _file) {
  for(unsigned i = 0; i < graph->maxArcId(); i++) {
    SmartDigraph::Arc arc = graph->arcFromId(i);
    _file << graph->id(graph->source(arc)) << "->" << graph->id(graph->target(arc)) << "\n";
  }
}
