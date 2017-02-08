//============================================================================
// Name        : model.cpp
// Author      : Sávio S. Dias
// E-mail      : diasssavio@gmail.com
// Institution : Universidade Federal Fluminense
// Description : Model class implementation
//============================================================================

#include "../../include/exact/model.h"

model::model(IloEnv _env, instance& _cars) : IloModel(_env), cars(_cars) {
	init();
	add_obj();
	add_const();
}

model::~model() { }

void model::init(){
	int n = cars.get_n();
	int c = cars.get_c();

	x = IloNumVarArray3(getEnv(), c);
	y = IloNumVarArray3(getEnv(), c);
	for(int k = 0; k < c; k++) {
		x[k] = IloNumVarArray2(getEnv(), n);
		y[k] = IloNumVarArray2(getEnv(), n);
		for(int i = 0; i < n; i++) {
			x[k][i] = IloNumVarArray(getEnv(), n);
			y[k][i] = IloNumVarArray(getEnv(), n);
			for(int j = 0; j < n; j++) {
        if(i == j)
          x[k][i][j] = IloNumVar(getEnv(), 0, 0, ILOINT);
          // x[k][i][j] = IloNumVar(getEnv(), 0, 0, ILOFLOAT);
        else
          x[k][i][j] = IloNumVar(getEnv(), 0, 1, ILOINT);
				  // x[k][i][j] = IloNumVar(getEnv(), 0, 1, ILOFLOAT);
				stringstream x_name;
				x_name << "x(" << k << ")(" << i << ")(" << j << ")";
				x[k][i][j].setName(x_name.str().c_str());
				add(x[k][i][j]);

        if(i == j && i != 0)
          y[k][i][j] = IloNumVar(getEnv(), 0, 0, ILOINT);
          // y[k][i][j] = IloNumVar(getEnv(), 0, 0, ILOFLOAT);
        else
				  y[k][i][j] = IloNumVar(getEnv(), 0, 1, ILOINT);
				  // y[k][i][j] = IloNumVar(getEnv(), 0, 1, ILOFLOAT);
				stringstream y_name;
				y_name << "y(" << k << ")(" << i << ")(" << j << ")";
				y[k][i][j].setName(y_name.str().c_str());
				add(y[k][i][j]);
			}
		}
	}
}

void model::add_const(){
	int n = cars.get_n();
	int c = cars.get_c();

	IloEnv env = getEnv();

	// Constraint (2): a vehicle must be rented at the beginning
	IloExpr expr(env);
	for(int k = 0; k < c; k++)
		for(int j = 0; j < n; j++)
			expr += y[k][0][j];
	IloConstraint c2 = (expr == 1);
	stringstream c2_name;
	c2_name << "Cons2";
	c2.setName(c2_name.str().c_str());
	add(c2);

	// Constraint (3): a vehicle must be returned at the beginning
	IloExpr expr1(env);
	for(int k = 0; k < c; k++)
		for(int i = 0; i < n; i++)
			expr1 += y[k][i][0];
	IloConstraint c3 = (expr1 == 1);
	stringstream c3_name;
	c3_name << "Cons3";
	c3.setName(c3_name.str().c_str());
	add(c3);

	// Constraints (5): at most a vehicle can be rented in i and returned in j
	for(int car = 0; car < c; car++) {
		IloExpr expr(env);
		for(int i = 0; i < n; i++)
			for(int j = 0; j < n; j++)
				expr += y[car][i][j];
		IloConstraint c5 = (expr <= 1);
		stringstream c5_name;
		c5_name << "Cons5(" << car << ")";
		c5.setName(c5_name.str().c_str());
		add(c5);
	}

	// Constraints (6)-(7): each vertex must be visited by a vehicle only
	for(int j = 0; j < n; j++) {
		IloExpr expr(env);
		for(int k = 0; k < c; k++)
			for(int i = 0; i < n; i++) {
				if(i == j) continue;
				expr += x[k][i][j];
			}
		IloConstraint c6 = (expr == 1);
		stringstream c6_name;
		c6_name << "Cons6(" << j << ")";
		c6.setName(c6_name.str().c_str());
		add(c6);
	}
	for(int i = 0; i < n; i++) {
		IloExpr expr(env);
		for(int k = 0; k < c; k++)
			for(int j = 0; j < n; j++) {
				if(i == j) continue;
				expr += x[k][i][j];
			}
		IloConstraint c7 = (expr == 1);
		stringstream c7_name;
		c7_name << "Cons7(" << i << ")";
		c7.setName(c7_name.str().c_str());
		add(c7);
	}

	// Constraints (8): flow through vehicle path
	for(int car = 0; car < c; car++)
		for(int i = 0; i < n; i++) {
			IloExpr expr(env);
        for(int j = 0; j < n; j++) {
					if(j == i) continue;
					expr += x[car][i][j];
				}
        for(int j = 0; j < n; j++) {
					if(j == i) continue;
					expr -= x[car][j][i];
				}

			IloExpr expr2(env);
        for(int l = 0; l < n; l++) {
          if(i == l) continue;
          expr2 += y[car][i][l];
        }
        for(int l = 0; l < n; l++) {
          if(i == l) continue;
				  expr2 -= y[car][l][i];
        }
			IloConstraint c8 = (expr == expr2);
			stringstream c8_name;
      c8_name << "Cons8(" << car << ")(" << i << ")";
			c8.setName(c8_name.str().c_str());
			add(c8);
		}

    // Constraints (9)-(10): An edge must be used by a vehicle only if it is rented somewhere
		/*for(int k = 0; k < c; k++)
      for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++) {
          IloExpr expr(env);
          for(int f = 0; f < n; f++)
            for(int l = 0; l < n; l++)
              expr += y[k][f][l];
          IloConstraint c9 = (x[k][i][j] <= expr);
    			stringstream c9_name;
    			c9_name << "Cons9(" << k << ")(" << i << ")(" << j << ")";
    			c9.setName(c9_name.str().c_str());
    			add(c9);
        }
    for(int car = 0; car < c; car++)
      for(int k = 0; k < n; k++)
        for(int l = 0; l < n; l++) {
          IloExpr expr(env);
          for(int i = 0; i < n; i++)
            for(int j = 0; j < n; j++)
              expr += x[car][i][j];
          IloConstraint c10 = (y[car][k][l] <= expr);
    			stringstream c10_name;
    			c10_name << "Cons10(" << car << ")(" << k << ")(" << l << ")";
    			c10.setName(c10_name.str().c_str());
    			add(c10);
        }*/
    // Luidi's improvements for Constraints (9)-(10)
    for(int car = 0; car < c; car++)
      for(int i = 0; i < n; i++) {
        IloExpr expr1(env);
        for(int j = 0; j < n; j++) {
          if(i == j) continue;
          expr1 += x[car][j][i];
        }
        IloExpr expr2(env);
        for(int k = 0; k < n; k++)
          for(int l = 0; l < n; l++)
            expr2 += y[car][k][l];
        for(int j = 0; j < n; j++) {
          if(i == j) continue;
          expr2 -= y[car][i][j];
        }
        IloConstraint c9 = (expr1 <= expr2);
        stringstream c9_name;
        c9_name << "Cons9(" << car << ")(" << i << ")";
        c9.setName(c9_name.str().c_str());
        add(c9);
      }
    for(int car = 0; car < c; car++)
      for(int j = 0; j < n; j++) {
        IloExpr expr1(env);
        for(int i = 0; i < n; i++) {
          if(i == j) continue;
          expr1 += x[car][j][i];
        }
        IloExpr expr2(env);
        for(int k = 0; k < n; k++)
          for(int l = 0; l < n; l++)
            expr2 += y[car][k][l];
        for(int i = 0; i < n; i++) {
          if(i == j) continue;
          expr2 -= y[car][i][j];
        }
        IloConstraint c10 = (expr1 <= expr2);
        stringstream c10_name;
        c10_name << "Cons10(" << car << ")(" << j << ")";
        c10.setName(c10_name.str().c_str());
        add(c10);
      }

    // Constraints (11)-(12): Eliminating the possibility of visiting a renting or leaving a returning place from the current vehicle using it
		/*for(int j = 0; j < n; j++)
			for(int k = 0; k < c; k++) {
				IloExpr expr1(env);
				for(int i = 0; i < n; i++) {
					if(i == j) continue;
					expr1 += x[k][j][i];
				}
				IloExpr expr2(env);
				for(int i = 0; i < n; i++) {
					if(i == j) continue;
					expr2 += y[k][i][j];
				}

				IloConstraint c11 = (expr1 <= 1 - expr2);
				stringstream c11_name;
				c11_name << "Cons11(" << j << ")(" << k << ")";
				c11.setName(c11_name.str().c_str());
				add(c11);
			}
		for(int i = 0; i < n; i++)
			for(int k = 0; k < c; k++) {
				IloExpr expr1(env);
				for(int j = 0; j < n; j++) {
					if(i == j) continue;
					expr1 += x[k][j][i];
				}
				IloExpr expr2(env);
				for(int j = 0; j < n; j++) {
					if(i == j) continue;
					expr2 += y[k][i][j];
				}

				IloConstraint c12 = (expr1 <= 1 - expr2);
				stringstream c12_name;
				c12_name << "Cons12(" << i << ")(" << k << ")";
				c12.setName(c12_name.str().c_str());
				add(c12);
			}*/
  // Luidi's improvements for Constraints (11)-(12)
  for(int car = 0; car < c; car++)
    for(int k = 0; k < n; k++) {
      IloExpr expr1(env);
      for(int j = 0; j < n; j++)
        expr1 += y[car][k][j];
      IloExpr expr2(env);
      for(int j = 0; j < n; j++)
        expr2 += x[car][k][j];
      IloConstraint c11 = (expr1 <= expr2);
      stringstream c11_name;
      c11_name << "Cons11(" << car << ")(" << k << ")";
      c11.setName(c11_name.str().c_str());
      add(c11);
    }
  for(int car = 0; car < c; car++)
    for(int l = 0; l < n; l++) {
      IloExpr expr1(env);
      for(int i = 0; i < n; i++)
        expr1 += y[car][i][l];
      IloExpr expr2(env);
      for(int i = 0; i < n; i++)
        expr2 += x[car][i][l];
      IloConstraint c12 = (expr1 <= expr2);
      stringstream c12_name;
      c12_name << "Cons12(" << car << ")(" << l << ")";
      c12.setName(c12_name.str().c_str());
      add(c12);
    }
}

void model::add_obj() {
	int n = cars.get_n();
	int c = cars.get_c();
	vector< matrix_2d > distances = cars.get_distances();
	vector< matrix_2d > return_rates = cars.get_return_rates();

	// Creating OF expression
	IloExpr expr(getEnv());
	for(int k = 0; k < c; k++)
		for(int i = 0; i < n; i++)
			for(int j = 0; j < n; j++)
				expr += (distances[k][i][j] * x[k][i][j]) + (return_rates[k][i][j] * y[k][i][j]);

	// Adding objective function
	add(IloMinimize(getEnv(), expr));
}

mincut_model::mincut_model(IloEnv env, instance& _cars, IloNumArray2& _x, IloNumArray2& _y) : IloModel(env), cars(_cars), x(_x), y(_y) {
  init();
	add_obj();
	add_const();
}

mincut_model::~mincut_model() { }

void mincut_model::init(){
	int n = cars.get_n();
	int c = cars.get_c();

  z = IloNumVarArray2(getEnv(), n);
  u = IloNumVarArray(getEnv(), n);
  for(int i = 0; i < n; i++) {
    z[i] = IloNumVarArray(getEnv(), n);
    u[i] = IloNumVar(getEnv(), 0, 1, ILOINT);
    for(int j = 0; j < n; j++) {
      z[i][j] = IloNumVar(getEnv(), 0, 1, ILOINT);
      stringstream z_name;
      z_name << "z(" << i << ")(" << j << ")";
      z[i][j].setName(z_name.str().c_str());
      add(z[i][j]);
    }

    stringstream u_name;
    u_name << "u(" << i << ")";
    u[i].setName(u_name.str().c_str());
    add(u[i]);
  }
}

void mincut_model::add_const(){
  int n = cars.get_n();
  IloEnv env = getEnv();

  for(int i = 0; i < n; i++)
    for(int j = 0; j < n; j++) {
      IloConstraint c1 = (z[i][j] >= u[j] - u[i]);
    	stringstream c1_name;
    	c1_name << "Cons1";
    	c1.setName(c1_name.str().c_str());
    	add(c1);
    }

  for(int i = 0; i < n; i++)
    for(int j = 0; j < n; j++) {
      IloConstraint c2 = (z[i][j] <= u[j]);
    	stringstream c2_name;
    	c2_name << "Cons2";
    	c2.setName(c2_name.str().c_str());
    	add(c2);
    }

  for(int i = 0; i < n; i++)
    for(int j = 0; j < n; j++) {
      IloConstraint c3 = (z[i][j] <= 1 - u[i]);
    	stringstream c3_name;
    	c3_name << "Cons3";
    	c3.setName(c3_name.str().c_str());
    	add(c3);
    }
}

void mincut_model::add_obj() {
  int n = cars.get_n();

  // Creating OF expression
  IloExpr expr(getEnv());
  for(int i = 0; i < n; i++)
    for(int j = 0; j < n; j++)
      expr += y[i][j] * z[i][j];

  for(int i = 0; i < n; i++)
    for(int j = 0; j < n; j++) {
      if(i == j) continue;
      expr -= x[i][j] * z[i][j];
    }

  // Adding objective function
	add(IloMaximize(getEnv(), expr));
}
