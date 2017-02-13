//============================================================================
// Name        : model.cpp
// Author      : Sávio S. Dias
// E-mail      : diasssavio@gmail.com
// Institution : Universidade Federal Fluminense
// Description : Model class implementation
//============================================================================

#include "../../include/exact/model.h"

model::model(IloEnv& _env, instance& _cars, IloNumArray2& _B, IloNumArray2& _f, graph& _g) : IloModel(_env), cars(_cars), B(_B), f(_f), g(_g) {
	// g = _g;
	init();
	add_obj();
	add_const();
}

model::~model() { }

void model::init(){
	int c = cars.get_c();

	lambda = IloNumVarArray2(getEnv(), g.n_nodes);
	for(int i = 0; i < g.n_nodes; i++) {
		lambda[i] = IloNumVarArray(getEnv(), c);
		for(int k = 0; k < c; k++) {
      lambda[i][k] = IloNumVar(getEnv(), 0, 1, ILOINT);
      // lambda[i][k] = IloNumVar(getEnv(), 0, 1, ILOFLOAT);

			stringstream lambda_name;
			lambda_name << "lambda(" << i << ")(" << k << ")";
			lambda[i][k].setName(lambda_name.str().c_str());
			add(lambda[i][k]);
		}
	}
	chi = IloNumVarArray(getEnv(), g.n_arcs);
	for(int i = 0; i < g.n_arcs; i++) {
		chi[i] = IloNumVar(getEnv(), 0, 1, ILOINT);
		stringstream chi_name;
		chi_name << "chi(" << i << ")";
		chi[i].setName(chi_name.str().c_str());
		add(chi[i]);
	}
}

void model::add_const(){
	int n = cars.get_n();
	int c = cars.get_c();
	int n_a = g.n_nodes;

	IloEnv env = getEnv();

	// Constraint (9.2): The number of trips cannot exceed the number of vehicles type
	IloExpr expr(env);
  for(int i = 1; i < n_a; i++)
    for(int k = 0; k < c; k++)
  		expr += lambda[i][k];
	IloConstraint c2 = (expr <= c);
	stringstream c2_name;
	c2_name << "Cons2";
	c2.setName(c2_name.str().c_str());
	add(c2);

	// Constraint (9.3):
	for(int i = 1; i < n_a; i++) {
		IloExpr expr(env);
		for(int k = 0; k < c; k++)
			expr += lambda[i][k];

		IloConstraint c3 = (expr <= 1);
		stringstream c3_name;
		c3_name << "Cons3(" << i << ")";
		c3.setName(c3_name.str().c_str());
		add(c3);
	}

	// Constraint (9.4):
	for(int k = 0; k < c; k++) {
		IloExpr expr(env);
		for(int i = 1; i < n_a; i++)
			expr += lambda[i][k];

		IloConstraint c4 = (expr <= 1);
		stringstream c4_name;
		c4_name << "Cons4(" << k << ")";
		c4.setName(c4_name.str().c_str());
		add(c4);
	}

	// Constraint (9.5):
	for(int j = 0; j < n; j++) {
		IloExpr expr(env);
		for(int i = 1; i < n_a; i++)
			for(int k = 0; k < c; k++)
				expr += B[i-1][j] * lambda[i][k];

		IloConstraint c5 = (expr == 1);
		stringstream c5_name;
		c5_name << "Cons5(" << j << ")";
		c5.setName(c5_name.str().c_str());
		add(c5);
	}

	// Constraint (9.6):
	// for(int i = 1; i < n_a; i++) {
	// 	IloExpr expr(env);
	// 	for(int k = 0; k < c; k++)
	// 		expr += lambda[i][k];
  //
	// 	IloConstraint c6 = (expr >= mu[i]);
	// 	stringstream c6_name;
	// 	c6_name << "Cons6(" << i << ")";
	// 	c6.setName(c6_name.str().c_str());
	// 	add(c6);
	// }

  // Constraint (9.7):
  // IloExpr expr2(env);
  // for(int k = 0; k < c; k++)
  //   expr2 += lambda[0][k];
  IloConstraint c7 = (lambda[0][0] == 1);
  stringstream c7_name;
  c7_name << "Cons7";
  c7.setName(c7_name.str().c_str());
  add(c7);
}

void model::add_obj() {
	int n_a = g.n_nodes;
	int c = cars.get_c();

	// Creating OF expression
	IloExpr expr(getEnv());
	for(int i = 1; i < n_a; i++)
		for(int k = 0; k < c; k++)
			expr += f[i-1][k] * lambda[i][k];

	// Adding objective function
	add(IloMinimize(getEnv(), expr));
}
