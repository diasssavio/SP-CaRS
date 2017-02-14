//============================================================================
// Name        : callbacks.h
// Author      : Sávio S. Dias
// E-mail      : diasssavio@gmail.com
// Institution : Universidade Federal Fluminense
// Description : Callbacks class interface
//============================================================================

#ifndef SRC_CALLBACKS_H_
#define SRC_CALLBACKS_H_

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <set>
#include <cmath>
#include <ilcplex/ilocplex.h>

#include "../FWChrono.h"
#include "../instance.h"
#include "../typedef.hpp"
#include "../solution.h"
// #include "mincut.h"
#include "gmindcut.h"
#include "model.h"
#include "node_info.h"

// **** DEPRECATED: Solve callback for gathering LR informations
class LR_callback : public IloCplex::SolveCallbackI {
private:
  FWChrono& timer;

  // Is it at root node?
  bool root_node;

  // Linear Relaxation Objective Value & Time against
  IloNum* linear_obj;
  double* linear_time;

public:
  IloCplex::CallbackI* duplicateCallback() const {
    return new (getEnv()) LR_callback(*this);
  }

  LR_callback(IloEnv env, FWChrono& _timer, IloNum* _linear_obj, double* _linear_time) : IloCplex::SolveCallbackI(env), timer(_timer), linear_obj(_linear_obj), linear_time(_linear_time) {
    root_node = true;
  }

  void main();

};

// Callback to gather information in every node of optimization
class mipinfo_callback : public IloCplex::MIPInfoCallbackI {
private:
  FWChrono& timer;

  // Linear Relaxation Objective Value & Time against
  IloNum* linear_obj;
  double* linear_time;

public:
  IloCplex::CallbackI* duplicateCallback() const {
    return new (getEnv()) mipinfo_callback(*this);
  }

  mipinfo_callback(IloEnv env, FWChrono& _timer, IloNum* _linear_obj, double* _linear_time) : IloCplex::MIPInfoCallbackI(env), timer(_timer), linear_obj(_linear_obj), linear_time(_linear_time) {}

  void main();

};

// Lazy callback for agregated subcycle elimination
class subset_callback : public IloCplex::LazyConstraintCallbackI {
private:
  IloNumVarArray3& x;
  instance& cars;
  ofstream& _file;

public:
  IloCplex::CallbackI* duplicateCallback() const {
    return new (getEnv()) subset_callback(*this);
  }

  subset_callback(IloEnv env, IloNumVarArray3& _x, instance& _cars, ofstream& file) : IloCplex::LazyConstraintCallbackI(env), x(_x), cars(_cars), _file(file) { }

  void main();

};

// Cut callback for cut set constraints, i.e. applied in relaxed solutions
class hao_cutsetcallback : public IloCplex::UserCutCallbackI {
private:
  IloNumVarArray2& lambda;
  IloNumVarArray& chi;
  vector< trip >& trips;
  ofstream& _file;

  vector< str_edge >& edges;
  unsigned c;

  unsigned n_cuts;
  double sep_time;

public:
  IloCplex::CallbackI* duplicateCallback() const {
    return new (getEnv()) hao_cutsetcallback(*this);
  }

  hao_cutsetcallback(IloEnv env, IloNumVarArray2& _lambda, IloNumVarArray& _chi, vector< trip>& _trips, vector< str_edge >& _edges, ofstream& file, unsigned _c) : IloCplex::UserCutCallbackI(env), lambda(_lambda), chi(_chi), trips(_trips), edges(_edges), _file(file), c(_c) {
    n_cuts = 0;
    sep_time = 0.0;
  }

  void main();

  inline unsigned get_n_cuts() { return n_cuts; }
  inline double get_sep_time() { return sep_time; }

};

// Lazy callback for cut set constraints, i.e. applied in integer solutions
class hao_cutsetcallback2 : public IloCplex::LazyConstraintCallbackI {
private:
  IloNumVarArray2& lambda;
  IloNumVarArray& chi;
  vector< trip >& trips;
  ofstream& _file;

  vector< str_edge >& edges;
  unsigned c;

  unsigned n_cuts;
  double sep_time;

public:
  IloCplex::CallbackI* duplicateCallback() const {
    return new (getEnv()) hao_cutsetcallback2(*this);
  }

  hao_cutsetcallback2(IloEnv env, IloNumVarArray2& _lambda, IloNumVarArray& _chi, vector< trip>& _trips, vector< str_edge >& _edges, ofstream& file, unsigned _c) : IloCplex::LazyConstraintCallbackI(env), lambda(_lambda), chi(_chi), trips(_trips), edges(_edges), _file(file), c(_c) {
    n_cuts = 0;
    sep_time = 0.0;
  }

  void main();

  inline unsigned get_n_cuts() { return n_cuts; }
  inline double get_sep_time() { return sep_time; }

};

#endif /* SRC_CALLBACKS_H_ */
