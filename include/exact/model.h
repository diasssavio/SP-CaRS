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
#include <ilcplex/ilocplex.h>

#include "instance.h"
#include "typedef.hpp"

class model: public IloModel {
private:
	void init(); // Initialize all variables for problem
	void add_const(); // Add constraints to model
	void add_obj(); // Add objective function to model

public:
	model(IloEnv, instance&);
	virtual ~model();

	// Instance
	instance& cars;

	// Variables
	// Cycle
	IloNumVarArray3 x;

	// Renting & returning places
	IloNumVarArray3 y;
};

class mincut_model: public IloModel {
private:
	void init(); // Initialize all variables for problem
	void add_const(); // Add constraints to model
	void add_obj(); // Add objective function to model

public:
	mincut_model(IloEnv, instance&, IloNumArray2&, IloNumArray2&);
	virtual ~mincut_model();

  // Instance
  instance& cars;

  // Variables
  IloNumVarArray2 z;
  IloNumVarArray u;

	// Objective function coefficients
	// Cycle, Renting & returning places
	IloNumArray2 x;
	IloNumArray2 y;
};

#endif /* SRC_MODEL_H_ */
