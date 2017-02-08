//============================================================================
// Name        : gmindcut.h
// Author      : Sávio S. Dias
// E-mail      : diasssavio@gmail.com
// Institution : Universidade Federal Fluminense
// Description : Gmindcut class interface
//============================================================================

#ifndef SRC_GMINDCUT_H_
#define SRC_GMINDCUT_H_

#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>

#include "typedef.hpp"

using std::vector;
using std::cout;
using std::ofstream;

#define EPS 1e-7

class Gmindcut {
private:
  graph gr;
  vector<int> number;
  // static double tempo;
  static int nx;
  static int ncut;
  ofstream& _file;

  void initialize();
  int bfs0(node *t);
  void bfs1(node *t);
  void reverse_arcs();
  void alloc_graph(const int, const int);
  void de_alloc_graph();
  bool gmincut(double&, int&, int&, const bool, vector<str_cut*>&);
  void add_cut(int&, vector<str_cut*>&, const int, const double) const;
  // double calcula_tempo(const unsigned long, const unsigned long) const;
  bool verif_corte(const double, const int, const bool) const;
  void generate_cuts(vector<vector<int> >&, int&, vector<str_cut*>&, const bool);
  void del_cuts(int&, vector<str_cut*>&) const;

public:
  Gmindcut(ofstream&);
  virtual ~Gmindcut();

  void start(const int, const int, const vector<str_edge>&);
  void write_graph() const;
  int generate_cuts(const vector<double>&, vector<vector<int> >&, const vector<str_edge>&, const bool);

  // double get_tempo() const { return tempo; }
  int get_nx() const { return nx; }
  int get_ncut() const { return ncut; }

};

#endif /* SRC_GMINDCUT_H_ */
