//============================================================================
// Name        : model.h
// Author      : Sávio S. Dias
// E-mail      : diasssavio@gmail.com
// Institution : Universidade Federal Fluminense
// Description : Model class interface
//============================================================================

#ifndef SRC_MODEL_H_
#define SRC_MODEL_H_

#include <sstream>
#include <vector>
#include <ilcplex/ilocplex.h>

#include "../instance.h"
#include "../typedef.hpp"

class model: public IloModel {
private:
	void init(); // Initialize all variables for problem
	void add_const(); // Add constraints to model
	void add_obj(); // Add objective function to model

	// Set Partition coefficients
	IloNumArray2 B; // Which vertices are in which trip
	IloNumArray2 f; // Cost of each trip using a different vehicle
	graph g;

public:
	model(IloEnv&, instance&, IloNumArray2&, IloNumArray2&, graph&);
	virtual ~model();

	// Instance
	instance& cars;

	// Variables
	IloNumVarArray2 lambda; // Whether a trip is realized by a vehicle
	IloNumVarArray chi; // Whether an arc is in the cycle (solution)
	IloNumVarArray mu; // Whether a trip is in the cycle (solution)
};

#endif /* SRC_MODEL_H_ */
