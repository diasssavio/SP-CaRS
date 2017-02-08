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
#include "../typedef.hpp"
#include "../instance.h"

class solver : public IloCplex {
private:
	IloNumArray3 x;
	IloNumArray3 y;

	model& mod;

	double obj_value;

public:
	solver(model&);

	virtual ~solver();

	void run(double = MAX_DOUBLE, double = MAX_DOUBLE, bool = false);

	IloNumArray3& get_x() { return x; }
	IloNumArray3& get_y() { return y; }
	double get_obj_value() const { return obj_value; }
};

#endif /* SRC_SOLVER_H_ */
