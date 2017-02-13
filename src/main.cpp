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
  // B.add(IloNumArray(env));
  // f.add(IloNumArray(env));
  for(vector< trip >::iterator i = trips.begin(); i < trips.end(); i++) {
    B.add(i->vertices_coeff(env));
    f.add(i->trip_costs(env, cars));
  }

  // printf("TRIPS ADDED:\n");
  // for(vector< trip >::iterator i = trips.begin(); i < trips.end(); i++)
  //   i->show_data();

  vector< str_edge > ar = build_graph(trips);
  graph g;
  g.n_nodes = trips.size() + 1;
  g.n_arcs = ar.size();
  // printf("EDGES ASSIGNED:\n");
  // for(vector< str_edge >::iterator it = ar.begin(); it < ar.end(); it++)
  //   printf("%d->%d\n", it->v1, it->v2);

  try {
    model mod(env, cars, B, f, g);
    solver cplex(env, mod, timer);
    cplex.run();

    printf("OBJ VALUE: %.2lf\n", cplex.get_obj_value());
    IloNumArray2 lambda = cplex.get_lambda();
    for(unsigned i = 0; i < g.n_nodes; i++) {
      for(unsigned k = 0; k < cars.get_c(); k++)
        printf("%4.0lf", lambda[i][k]);
      printf("\n");
    }
  } catch (IloException& e) {
    cerr << "CONCERT EXCEPTION -- " << e << endl;
  }

	return 0;
}

vector< str_edge > build_graph(vector< trip >& trips) {
  vector< str_edge > edges;
  for(unsigned i = 0; i < trips.size(); i++) {
    // Adding edges starting from v0
    if(trips[i].get_renting() == 0) {
      str_edge aux = {0, i + 1};
      edges.push_back(aux);
    }

    // Adding edges finishing in v0
    if(trips[i].get_returning() == 0) {
      str_edge aux = {i + 1, 0};
      edges.push_back(aux);
    }

    // Adding edges between trips
    BitArray* trip_vertices = trips[i].get_bit_vertices();
    for(unsigned j = 0; j < trips.size(); j++) {
      if(i == j) continue;
      if(trips[i].get_returning() == trips[j].get_renting() && trips[i].get_returning() != 0)
        if(!trip_vertices->has_commom_element(trips[j].get_bit_vertices())) {
          str_edge aux = {i + 1, j + 1};
          edges.push_back(aux);
        }
    }
  }

  return edges;
}
