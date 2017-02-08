//============================================================================
// Name        : solver.cpp
// Author      : Sávio S. Dias
// E-mail      : diasssavio@gmail.com
// Institution : Universidade Federal Fluminense
// Description : Solver class implementation
//============================================================================

#include "../../include/exact/solver.h"

solver::solver(IloEnv& _env, model& _mod, FWChrono& _timer) : env(_env), mod(_mod), IloCplex(_mod), timer(_timer) {
  linear_obj = 0.0;
  linear_time = 0.0;
}

solver::~solver() { }

void solver::set_params(double tl) {
  setParam(IloCplex::Threads, 1);
  // setParam(IloCplex::NodeLim, 0);
  setParam(IloCplex::Param::Preprocessing::Aggregator, 0);
  setParam(IloCplex::Param::Preprocessing::Presolve, 0);
  setParam(IloCplex::PreInd, 0);
  setParam(IloCplex::CutsFactor, 1);
  setParam(IloCplex::HeurFreq, -1);
  setParam(IloCplex::Param::TimeLimit, tl);
}

void solver::run(double tl, double UB, bool first) {
  set_params(tl);

  ofstream _file;
  _file.open("cutset.txt", ios::trunc);

  // exportModel("test.lp");

  // use(IloCplex::Callback(new (env) hao_cutsetcallback(env, mod.x, cars, _file)));
  // use(IloCplex::Callback(new (env) hao_cutsetcallback2(env, mod.x, cars, _file)));
  // use(IloCplex::Callback(new (env) branch_callback(env, timer, cars, mod.x, mod.y, &linear_obj, &linear_time)));
  use(IloCplex::Callback(new (env) mipinfo_callback(env, timer, &linear_obj, &linear_time)));

  solve();
  timer.stop();
  _file.close();

  obj_value = getObjValue();
}
