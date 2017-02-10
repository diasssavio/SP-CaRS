//============================================================================
// Name        : main.cpp
// Author      : Sávio S. Dias
// E-mail      : diasssavio@gmail.com
// Institution : Universidade Federal Fluminense
// Description : Main program file
//============================================================================

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <ilcplex/ilocplex.h>

#include "../include/FWChrono.h"
#include "../include/mt19937ar.h"
#include "../include/instance.h"
#include "../include/solution.h"
#include "../include/heuristic/logger.h"
#include "../include/heuristic/constructor.h"
#include "../include/heuristic/ils.h"
#include "../include/exact/model.h"
#include "../include/exact/solver.h"

using namespace std;

template<typename T>
T string_to(const string& s){
	istringstream i(s);
	T x;
	if (!(i >> x)) return 0;
	return x;
}

vector< str_edge > build_graph(vector< trip >&);

ILOSTLBEGIN

int main(int argc, char* args[]) {
	FWChrono timer;
	timer.start();

	unsigned long seed = time(NULL);
	if(argc >= 3)
		seed = string_to< unsigned long >(args[2]);
	// time_t seed = time(NULL);
	// srand(seed);
	init_genrand(seed);

	instance cars;
	// cars.read_data();
  cars.read_from_file(args[1]);

  double alpha = 0.47;
	unsigned max_it = 61;
	unsigned max_ms_it = 28;

  // double alpha = 0.25;
	// unsigned max_it = 50;
	// unsigned max_ms_it = 20;

  if(argc >= 4) {
    alpha = string_to< double >(args[3]);
    max_it = string_to< unsigned >(args[4]);
    max_ms_it = string_to< unsigned >(args[5]);
  }

  // Executing MILS
	logger* logs = new logger(timer);
	ils ILS(cars, max_ms_it, max_it, alpha, logs);
	solution best = ILS.execute();
	timer.stop();
  printf("BEST %.0lf TIME %.2lf\n", best.get_cost(), timer.getStopTime());

  best.show_data();

  // Gathering trip data for Set Partition
  IloEnv env;
  vector< trip > trips = ILS.get_pool();
  IloNumArray2 B(env);
  IloNumArray2 f(env);
  for(vector< trip >::iterator i = trips.begin(); i < trips.end(); i++) {
    B.add(i->vertices_coeff(env));
    f.add(i->trip_costs(env, cars));
  }

  vector< str_edge > ar = build_graph(trips);

  try {
    model mod(env, cars, B, f, g);
    solver cplex(env, mod, timer);
    cplex.run();
  } catch (IloException& e) {
    cerr << "CONCERT EXCEPTION -- " << e << endl;
  }

	return 0;
}

vector< str_edge > build_graph(vector< trip >& trips) {

}
