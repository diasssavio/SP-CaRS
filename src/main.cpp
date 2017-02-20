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
#include "../include/exact/MinCut.h"
#include "../include/exact/solver.h"

using namespace std;

template<typename T>
T string_to(const string& s){
	istringstream i(s);
	T x;
	if (!(i >> x)) return 0;
	return x;
}

// vector< str_edge > build_graph(vector< trip >&);
ArcList build_graph(vector< trip >&);

ILOSTLBEGIN

ILOLAZYCONSTRAINTCALLBACK6(CtCallback, IloNumVarArray, chi, IloNumVarMatrix, lambda, instance&, cars, vector< trip >&, trips, ArcList*, edges, ofstream&, _file)
{
  bool logs = true;
	if(logs)
		_file << "\nEntering CtCallback. Solution value:" << getObjValue() << "\n";
	IloEnv env = getEnv();

	IloInt numVertices = trips.size() + 1;

	// IloNumMatrix sol_chi(env, numVertices);
  IloNumArray sol_chi(env, edges->size());
	IloNumArray sol_lambda(env, numVertices);

	for (int i = 0; i < numVertices; i++) {
    // sol_chi[i] = IloNumArray(env, numVertices);
    IloNum aux = 0.0;
    for(int k = 0; k < cars.get_c(); k++)
      aux += getValue(lambda[i][k]);
  	sol_lambda[i] = aux;
  }

	// for (ArcList::iterator it = edges->begin(); it != edges->end(); ++it)
	// {
	// 		_Arc * aux = *it;
	// 		sol_chi[aux->get_i()][aux->get_j()] = getValue(chi[aux->get_i()][aux->get_j()]);
	// }

  for (int it = 0; it < edges->size(); ++it)
		sol_chi[it] = getValue(chi[it]);

  if(logs)
  	_file << "Neighborhood list built!...\n";

	MinCut algo;
	// algo.run_maxflow(numVertices, edges->begin(), edges->end(), sol_chi);
  algo.run_maxflow(numVertices, edges, sol_chi);
	int *S = new int[numVertices];

	for (int id = 1; id <= trips.size(); ++id)
	{
		if (sol_lambda[id] > 1E-6) // o nó y está na solução
		{
			//calculando o valor do corte
			double val = algo.generate_min_cut(0, id);

      if(logs)
        _file << "Cut cost (0," << id << "): " << val << ".\n";

			bool rInSbar = algo.is_node_in_cut(0);

			if (val - sol_lambda[id] < -1E-6) // se existem componentes conexas separadas
			{
        if(logs)
  			   _file << "Min Cut violated! lambda[" << id << "]: " << sol_lambda[id] << ".\n";
				for (int i = 0; i <= trips.size(); i++)
					if (algo.is_node_in_cut(i) == rInSbar)
						S[i] = 0;
					else
						S[i] = 1;

        _file << "S: ";
        for(int i = 0; i <= trips.size(); i++)
          if(S[i]) _file << i << " ";

        _file << "\nS_bar: ";
        for(int i = 0; i <= trips.size(); i++)
          if(!S[i]) _file << i << " ";
        _file << endl;

				IloExpr lhs(env);
				// for(ArcList::iterator it = edges->begin(); it != edges->end(); ++it){
				// 	_Arc * aux = *it;
				// 	if(S[aux->get_j()] and !S[aux->get_i()])
				// 		lhs += chi[aux->get_i()][aux->get_j()];
				// }

        for(int it = 0; it < edges->size(); ++it){
					// _Arc * aux = edges[it];
					if(S[edges->at(it)->get_j()] and !S[edges->at(it)->get_i()])
						lhs += chi[it];
				}

				IloExpr rhs(env);
        for(int k = 0; k < cars.get_c(); k++)
          rhs += lambda[id][k];
        IloConstraint cut = (lhs >= rhs);
        cut = add(cut);
        if(logs)
          _file << cut << endl;
        cut.end();
			}
		}
	}

	delete[] S;

  if(logs)
		_file << "Leaving CtCallback.\n";
}

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

  printf("TRIPS ADDED (%d):\n", trips.size());
  for(vector< trip >::iterator i = trips.begin(); i < trips.end(); i++)
    i->show_data();

  // vector< str_edge > ar = build_graph(trips);
  ArcList ar = build_graph(trips);
  graph g;
  g.n_nodes = trips.size() + 1;
  g.n_arcs = ar.size();
  printf("EDGES ASSIGNED (%d):\n", ar.size());
  int count = 0;
  for(ArcList::iterator it = ar.begin(); it < ar.end(); it++) {
    _Arc * aux = *it;
    printf("%d: %d->%d\n", count++, aux->get_i(), aux->get_j());
  }

  try {
    model mod(env, cars, B, f, g);
    solver cplex(env, mod, timer);
    ofstream _file;
    _file.open("cutset.txt", ios::trunc);
    cplex.use(CtCallback(env, mod.chi, mod.lambda, mod.cars, trips, &ar, _file));
    // cplex.run(trips, ar);
    cplex.run(trips, &ar);
    _file.close();

    printf("OBJ VALUE: %.2lf\n", cplex.get_obj_value());
    IloNumArray2 lambda = cplex.get_lambda();
    for(unsigned i = 0; i < g.n_nodes; i++) {
      for(unsigned k = 0; k < cars.get_c(); k++)
        printf("%4.0lf", lambda[i][k]);
      printf("\n");
    }

    IloNumArray chi = cplex.get_chi();
    printf("CHI:\n");
    for(unsigned i = 0; i < g.n_arcs; i++)
      if(chi[i] > EPSILON)
        printf("%4d", i);
    printf("\n");
  } catch (IloException& e) {
    cerr << "CONCERT EXCEPTION -- " << e << endl;
  }

	return 0;
}

ArcList build_graph(vector< trip >& trips) {
  ArcList edges;
  for(unsigned i = 0; i < trips.size(); i++) {
    // Adding edges starting from v0
    if(trips[i].get_renting() == 0) {
      _Arc * aux = new _Arc(0, i + 1);
      edges.push_back(aux);
    }

    // Adding edges finishing in v0
    if(trips[i].get_returning() == 0) {
      _Arc * aux = new _Arc(i + 1, 0);
      edges.push_back(aux);
    }

    // Adding edges between trips
    BitArray* trip_vertices = trips[i].get_bit_vertices();
    for(unsigned j = 0; j < trips.size(); j++) {
      if(i == j) continue;
      if(trips[i].get_returning() == trips[j].get_renting() && trips[i].get_returning() != 0)
        if(!trip_vertices->has_commom_element(trips[j].get_bit_vertices())) {
          _Arc * aux = new _Arc(i + 1, j + 1);
          edges.push_back(aux);
        }
    }
  }

  return edges;
}

// vector< str_edge > build_graph(vector< trip >& trips) {
//   vector< str_edge > edges;
//   for(unsigned i = 0; i < trips.size(); i++) {
//     // Adding edges starting from v0
//     if(trips[i].get_renting() == 0) {
//       str_edge aux = {0, i + 1};
//       edges.push_back(aux);
//     }
//
//     // Adding edges finishing in v0
//     if(trips[i].get_returning() == 0) {
//       str_edge aux = {i + 1, 0};
//       edges.push_back(aux);
//     }
//
//     // Adding edges between trips
//     BitArray* trip_vertices = trips[i].get_bit_vertices();
//     for(unsigned j = 0; j < trips.size(); j++) {
//       if(i == j) continue;
//       if(trips[i].get_returning() == trips[j].get_renting() && trips[i].get_returning() != 0)
//         if(!trip_vertices->has_commom_element(trips[j].get_bit_vertices())) {
//           str_edge aux = {i + 1, j + 1};
//           edges.push_back(aux);
//         }
//     }
//   }
//
//   return edges;
// }
