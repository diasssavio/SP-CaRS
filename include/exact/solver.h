//============================================================================
// Name        : solver.h
// Author      : Sávio S. Dias
// E-mail      : diasssavio@gmail.com
// Institution : Universidade Federal Fluminense
// Description : Solver class interface
//============================================================================

#ifndef SRC_SOLVER_H_
#define SRC_SOLVER_H_

#include <ilcplex/ilocplex.h>

#include "model.h"
#include "callbacks.h"
#include "../typedef.hpp"
#include "../instance.h"
#include "../FWChrono.h"

class solver : public IloCplex {
private:
	IloNumArray2 lambda;
	IloNumArray chi;
  IloNumArray mu;

  FWChrono& timer;

  IloEnv& env;
	model& mod;

	double obj_value;
  double linear_obj;
  double linear_time;

  void set_params(double);

public:
	solver(IloEnv&, model&, FWChrono&);

	virtual ~solver();

	void run(double = MAX_DOUBLE, double = MAX_DOUBLE, bool = false);

	IloNumArray2& get_lambda() { return lambda; }
	IloNumArray& get_chi() { return chi; }
  IloNumArray& get_mu() { return mu; }
	double get_obj_value() const { return obj_value; }
  double get_linear_obj() const { return linear_obj; }
  double get_linear_time() const { return linear_time; }
};

#endif /* SRC_SOLVER_H_ */
