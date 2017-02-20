//============================================================================
// Name        : neighborhoods.cpp
// Author      : Sávio S. Dias
// E-mail      : diasssavio@gmail.com
// Institution : Universidade Federal Fluminense
// Description : Iterated Local Search class implementation
//============================================================================

#include "../../include/heuristic/ils.h"

ils::ils( instance& _cars, unsigned _ms_it, unsigned _it, double _alpha, logger* _logs ) : max_it(_it), max_ms_it(_ms_it), alpha(_alpha) {
	this->cars = _cars;
	this->logs = _logs;
}

ils::~ils() { }

void ils::subsets( unsigned i, unsigned n ) {
	if(i > n){
		// printf("%d subsets:",++cont);
    vector< unsigned > aux;
  	for(unsigned j = 1; j <= n; j++)
    	if(conj[j] == 1)
    		aux.push_back(j);
    sets.push_back(aux);
	}else{
  	conj[i] = 1;
  	subsets(i + 1, n);
  	conj[i] = 0;
  	subsets(i + 1, n);
  }
}

solution& ils::execute() {
	constructor cons(cars, alpha);
	neighborhoods neighbors(cars);
	perturbation pert(cars);
	solution sol(cars);

	vector< unsigned > vehicles;
	for(unsigned k = 0; k < cars.get_c(); k++)
		vehicles.push_back(k);

	bool first = true;
	for(unsigned i = 0; i < max_ms_it; i++) {
		sol = cons.generate_sol(vehicles);
		if(first) {
			best = sol;
			first = false;
		}
		for(unsigned k = 0; k < max_it; k++) {
			sol = neighbors.execute(sol);
			if(sol.get_cost() < best.get_cost())
				best = sol;

			// Trying to add solution trips to pool
			add_to_pool(&sol);

			if((k + 1) < max_it)
				sol = pert.execute(sol);
		}
		// Adding the best solution for MS iterations
		logs->make_log(best.get_cost());
	}

	// logs.print_log();
  // add_to_pool(&best);

	return best;
}

void ils::add_to_pool(solution* _to_add) {
	double current_cost = _to_add->get_cost();
	double best_cost = best.get_cost();
	double acceptance_gap = 0.1;

	if(((current_cost - best_cost) / best_cost) <= acceptance_gap) {
    vector< trip > trips = _to_add->get_trips();
    for(vector< trip >::iterator i = trips.begin(); i < trips.end(); i++) {
      bool has_identical = false;
      for(vector< trip >::iterator j = pool.begin(); j < pool.end(); j++)
        if(*i == *j) { // If two trips have same renting/returning & same hash
          has_identical = true;
          break;
        }

      if(!has_identical)
  	    pool.push_back(*i);
    }
	}
}
