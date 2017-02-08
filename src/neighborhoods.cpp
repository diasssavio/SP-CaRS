//============================================================================
// Name        : neighborhoods.cpp
// Author      : Sávio S. Dias
// E-mail      : diasssavio@gmail.com
// Institution : Universidade Federal Fluminense
// Description : Neighborhoods class implementation
//============================================================================

#include "../include/neighborhoods.h"

int myrandom( unsigned i ) { return genrand_int32() % i; }

neighborhoods::neighborhoods( instance& _cars, bool _logs ) {
	this->cars = _cars;
	this->logs = _logs;
}

neighborhoods::~neighborhoods() { }

solution neighborhoods::i_swap_one( solution& p_sol ) {
	unsigned n = cars.get_n();
	vector< t_vec > vehicles = p_sol.get_vehicles();
	vector< matrix_2d > distances = cars.get_distances();

	vector< pair< unsigned, unsigned> > v_pos = p_sol.get_pos();
	vector< unsigned > route(p_sol.get_route());

	// Evaluating all possible swaps
	unsigned i_swap = 0, j_swap = 0;
	double current_cost = p_sol.get_cost();
	for(unsigned k = 0; k < vehicles.size(); k++)
		for(unsigned i = v_pos[k].first + 1; i < v_pos[k].second - 1; i++)
			for(unsigned j = i + 1; j < v_pos[k].second; j++) {
				// Aux variable to calculate the last edge of the cycle
				unsigned aux = j + 1;
				if(j == (n - 1)) aux = 0;

				// Calculating the current cost for the swap change
				double cost = p_sol.get_cost();
				cost += distances[ vehicles[k].number ][ route[i - 1] ][ route[j] ];
				cost += distances[ vehicles[k].number ][ route[i] ][ route[aux] ];
				cost -= distances[ vehicles[k].number ][ route[i - 1] ][ route[i] ];
				cost -= distances[ vehicles[k].number ][ route[j] ][ route[aux] ];
				cost -= distances[ vehicles[k].number ][ route[i] ][ route[i + 1] ];

				// If i & j are adjacent
				if(j == i + 1)
					cost += distances[ vehicles[k].number ][ route[j] ][ route[i] ];
				else { // otherwise
					cost += distances[ vehicles[k].number ][ route[j] ][ route[i + 1] ];
					cost += distances[ vehicles[k].number ][ route[j - 1] ][ route[i] ];
					cost -= distances[ vehicles[k].number ][ route[j - 1] ][ route[j] ];
				}

				// If the cost is smaller than the current, the change is applied
				// cout << cost << " - " << neighbor.evaluate() << " = " << cost - neighbor.evaluate() << endl;
				if(cost < current_cost) {
					i_swap = i;
					j_swap = j;
					current_cost = cost;
				}
				// cout << "... executed!" << endl;
			}

	if(i_swap != j_swap) {
		if(logs)
			cout << "Applying i_swap_1" << endl;
		swap(route[i_swap], route[j_swap]);
		solution neighbor(cars);
		neighbor.set_route(route);
		neighbor.set_vehicles(vehicles);
		neighbor.set_pos(v_pos);
		neighbor.set_cost(current_cost);
		return neighbor;
	}

	return p_sol;
}

solution neighborhoods::i_swap_two( solution& p_sol ) {
	unsigned n = cars.get_n();
	vector< t_vec > vehicles = p_sol.get_vehicles();
	vector< matrix_2d > distances = cars.get_distances();

	vector< pair< unsigned, unsigned> > v_pos = p_sol.get_pos();
	vector< unsigned > route(p_sol.get_route());

	// Evaluating all possible swaps
	unsigned i_swap = 0, j_swap = 0;
	double current_cost = p_sol.get_cost();
	for(unsigned k = 0; k < vehicles.size(); k++)
		for(int i = v_pos[k].first + 1; i < (int)v_pos[k].second - 3; i++)
			for(unsigned j = i + 2; j < v_pos[k].second - 1; j++) {
				// Aux variable to calculate the last edge of the cycle
				unsigned aux = j + 2;
				if(j == (n - 2)) aux = 0;

				// Calculating the current cost for the swap change
				double cost = p_sol.get_cost();
				cost += distances[ vehicles[k].number ][ route[i - 1] ][ route[j] ];
				cost += distances[ vehicles[k].number ][ route[i + 1] ][ route[aux] ];
				cost -= distances[ vehicles[k].number ][ route[i - 1] ][ route[i] ];
				cost -= distances[ vehicles[k].number ][ route[j + 1] ][ route[aux] ];
				cost -= distances[ vehicles[k].number ][ route[i + 1] ][ route[i + 2] ];

				// If i & j are adjacent
				if(j == i + 2)
					cost += distances[ vehicles[k].number ][ route[j + 1] ][ route[i] ];
				else { // otherwise
					cost += distances[ vehicles[k].number ][ route[j + 1] ][ route[i + 2] ];
					cost += distances[ vehicles[k].number ][ route[j - 1] ][ route[i] ];
					cost -= distances[ vehicles[k].number ][ route[j - 1] ][ route[j] ];
				}

				// If the cost is smaller than the current, the change is applied
				// cout << cost << " - " << neighbor.evaluate() << " = " << cost - neighbor.evaluate() << endl;
				if(cost < current_cost) {
					i_swap = i;
					j_swap = j;
					current_cost = cost;
				}
				// cout << "... executed!" << endl;
			}

	if(i_swap != j_swap) {
		if(logs)
			cout << "Applying i_swap_2" << endl;
		swap(route[i_swap], route[j_swap]);
		swap(route[i_swap + 1], route[j_swap + 1]);
		solution neighbor(cars);
		neighbor.set_route(route);
		neighbor.set_vehicles(vehicles);
		neighbor.set_pos(v_pos);
		neighbor.set_cost(current_cost);
		return neighbor;
	}

	return p_sol;
}

solution neighborhoods::i_swap_three( solution& p_sol ) {
	unsigned n = cars.get_n();
	vector< t_vec > vehicles = p_sol.get_vehicles();
	vector< matrix_2d > distances = cars.get_distances();

	vector< pair< unsigned, unsigned> > v_pos = p_sol.get_pos();
	vector< unsigned > route(p_sol.get_route());

	// Evaluating all possible swaps
	unsigned i_swap = 0, j_swap = 0;
	double current_cost = p_sol.get_cost();
	for(unsigned k = 0; k < vehicles.size(); k++)
		for(int i = v_pos[k].first + 1; i < (int)v_pos[k].second - 5; i++)
			for(int j = i + 3; j < (int)v_pos[k].second - 2; j++) {
				// Aux variable to calculate the last edge of the cycle
				unsigned aux = j + 3;
				if(j == (n - 3)) aux = 0;

				// Calculating the current cost for the swap change
				double cost = p_sol.get_cost();
				cost += distances[ vehicles[k].number ][ route[i - 1] ][ route[j] ];
				cost += distances[ vehicles[k].number ][ route[i + 2] ][ route[aux] ];
				cost -= distances[ vehicles[k].number ][ route[i - 1] ][ route[i] ];
				cost -= distances[ vehicles[k].number ][ route[j + 2] ][ route[aux] ];
				cost -= distances[ vehicles[k].number ][ route[i + 2] ][ route[i + 3] ];

				// If i & j are adjacent
				if(j == i + 3)
					cost += distances[ vehicles[k].number ][ route[j + 2] ][ route[i] ];
				else { // otherwise
					cost += distances[ vehicles[k].number ][ route[j + 2] ][ route[i + 3] ];
					cost += distances[ vehicles[k].number ][ route[j - 1] ][ route[i] ];
					cost -= distances[ vehicles[k].number ][ route[j - 1] ][ route[j] ];
				}

				// If the cost is smaller than the current, the change is applied
				// cout << cost << " - " << neighbor.evaluate() << " = " << cost - neighbor.evaluate() << endl;
				if(cost < current_cost) {
					i_swap = i;
					j_swap = j;
					current_cost = cost;
				}
				// cout << "... executed!" << endl;
			}

	if(i_swap != j_swap) {
		if(logs)
			cout << "Applying i_swap_3" << endl;
		swap(route[i_swap], route[j_swap]);
		swap(route[i_swap + 1], route[j_swap + 1]);
		swap(route[i_swap + 2], route[j_swap + 2]);
		solution neighbor(cars);
		neighbor.set_route(route);
		neighbor.set_vehicles(vehicles);
		neighbor.set_pos(v_pos);
		neighbor.set_cost(current_cost);
		return neighbor;
	}

	return p_sol;
}

solution neighborhoods::i_two_opt( solution& p_sol ) {
	unsigned n = cars.get_n();
	vector< t_vec > vehicles = p_sol.get_vehicles();
	vector< matrix_2d > distances = cars.get_distances();

	vector< pair< unsigned, unsigned> > v_pos = p_sol.get_pos();
	vector< unsigned > route(p_sol.get_route());

	// Evaluating all possible swaps
	unsigned i_opt = 0, j_opt = 0;
	double current_cost = p_sol.get_cost();
	for(unsigned k = 0; k < vehicles.size(); k++)
		for(unsigned i = v_pos[k].first + 1; i < v_pos[k].second - 1; i++)
			for(unsigned j = i + 3; j < v_pos[k].second; j++) {
				// Aux variable to calculate the last edge of the cycle
				unsigned aux = j + 1;
				if(j == (n - 1)) aux = 0;

				// Calculating the current cost for the 2-opt change
				double cost = p_sol.get_cost();
				cost -= distances[ vehicles[k].number ][ route[i - 1] ][ route[i] ];
				cost -= distances[ vehicles[k].number ][ route[j] ][ route[aux] ];
				cost += distances[ vehicles[k].number ][ route[i - 1] ][ route[j] ];
				cost += distances[ vehicles[k].number ][ route[i] ][ route[aux] ];

				// If the cost is smaller than the current, the change is applied
				if(cost < current_cost) {
					i_opt = i;
					j_opt = j + 1;
					current_cost = cost;
				}
			}

	if(i_opt != j_opt) {
		if(logs)
			cout << "Applying two_opt" << endl;
		reverse(route.begin() + i_opt, route.begin() + j_opt);
		solution neighbor(cars);
		neighbor.set_route(route);
		neighbor.set_vehicles(vehicles);
		neighbor.set_pos(v_pos);
		neighbor.set_cost(current_cost);
		return neighbor;
	}

	return p_sol;
}

solution neighborhoods::i_reverse( solution& p_sol ) {
	unsigned n = cars.get_n();
	vector< t_vec > vehicles(p_sol.get_vehicles());
	vector< matrix_2d > distances = cars.get_distances();
	vector< matrix_2d > rates = cars.get_return_rates();

	if(vehicles.size() <= 2) return p_sol;

	vector< pair< unsigned, unsigned > > v_pos = p_sol.get_pos();
	vector< unsigned > route(p_sol.get_route());

	// Evaluating all trips but the edge ones
	unsigned k_rev = 0;
	double current_cost = p_sol.get_cost();
	for(unsigned k = 1; k < vehicles.size() - 1; k++) {
		// Aux variable to account for the last edge of the cycle
		unsigned aux = v_pos[k + 1].second;
		if(aux == n) aux = 0;
		unsigned aux2 = v_pos[k].second + 1;
		if(aux2 == n) aux2 = 0;

		// Calculating the current cost for the inner reverse change
		double cost = p_sol.get_cost();
		cost -= rates[vehicles[k].number][ route[v_pos[k].first] ][ route[v_pos[k].second] ];
		cost -= rates[vehicles[k - 1].number][ route[v_pos[k - 1].first] ][ route[v_pos[k - 1].second] ];
		cost -= rates[vehicles[k + 1].number][ route[v_pos[k + 1].first] ][ route[aux] ];
		cost -= distances[vehicles[k - 1].number][ route[v_pos[k].first - 1] ][ route[v_pos[k].first] ];
		cost -= distances[vehicles[k + 1].number][ route[v_pos[k].second] ][ route[aux2] ];
		cost += rates[vehicles[k].number][ route[v_pos[k].second] ][ route[v_pos[k].first] ];
		cost += rates[vehicles[k - 1].number][ route[v_pos[k - 1].first] ][ route[v_pos[k].second] ];
		cost += rates[vehicles[k + 1].number][ route[v_pos[k].first] ][ route[aux] ];
		cost += distances[vehicles[k - 1].number][ route[v_pos[k].first - 1] ][ route[v_pos[k].second] ];
		cost += distances[vehicles[k + 1].number][ route[aux2] ][ route[v_pos[k].first] ];

		// If the cost is smaller than the current, the change is applied
		if(cost < current_cost) {
			current_cost = cost;
			k_rev = k;
		}
	}

	if(k_rev) {
		if(logs)
			cout << "Applying reverse" << endl;
		reverse(route.begin() + v_pos[k_rev].first, route.begin() + (v_pos[k_rev].second + 1));
		vehicles[k_rev].begin = vehicles[k_rev - 1].end = route[ v_pos[k_rev].first ];
		vehicles[k_rev].end = vehicles[k_rev + 1].begin = route[ v_pos[k_rev].second ];

		solution neighbor(cars);
		neighbor.set_route(route);
		neighbor.set_vehicles(vehicles);
		neighbor.set_pos(v_pos);
		neighbor.set_cost(current_cost);
		return neighbor;
	}

	return p_sol;
}

solution neighborhoods::i_shift_one( solution& p_sol ) {
	unsigned n = cars.get_n();
	vector< t_vec > vehicles = p_sol.get_vehicles();
	vector< matrix_2d > distances = cars.get_distances();

	vector< pair< unsigned, unsigned> > v_pos(p_sol.get_pos());
	vector< unsigned > route(p_sol.get_route());

	// Evaluating all possible shifts
	unsigned i_shift = 0, j_shift = 0;
	double current_cost = p_sol.get_cost();
	for(unsigned k = 0; k < vehicles.size(); k++)
		for(unsigned i = v_pos[k].first + 1; i < v_pos[k].second; i++)
			for(unsigned j = v_pos[k].first; j < v_pos[k].second; j++) {
				// If i == j + 1 means that I'll try to insert i in its same position as before
				if(i == j || i == (j + 1)) continue;

				// Aux variable to calculate the last edge of the cycle
				unsigned aux1 = i + 1;
				if(i == (n - 1)) aux1 = 0;
				unsigned aux2 = j + 1;
				if(j == (n - 1)) aux2 = 0;

				// Calculating the current cost for the or-opt-1 change
				double cost = p_sol.get_cost();
				cost -= distances[ vehicles[k].number ][ route[i - 1] ][ route[i] ];
				cost -= distances[ vehicles[k].number ][ route[i] ][ route[aux1] ];
				cost -= distances[ vehicles[k].number ][ route[j] ][ route[aux2] ];
				cost += distances[ vehicles[k].number ][ route[j] ][ route[i] ];
				cost += distances[ vehicles[k].number ][ route[i] ][ route[aux2] ];
				cost += distances[ vehicles[k].number ][ route[i - 1] ][ route[aux1] ];

				// If the cost is smaller than the current, the change is applied
				if(cost < current_cost) {
					i_shift = i;
					j_shift = j;
					current_cost = cost;
				}
			}

	if(i_shift != j_shift) {
		if(logs)
			cout << "Applying i_shift_1" << endl;
		unsigned value = route[i_shift];
		if(i_shift > j_shift) {
			route.erase(route.begin() + i_shift);
			route.insert(route.begin() + (j_shift + 1), value);
		} else {
			route.insert(route.begin() + (j_shift + 1), value);
			route.erase(route.begin() + i_shift);
		}
		solution neighbor(cars);
		neighbor.set_route(route);
		neighbor.set_vehicles(vehicles);
		neighbor.set_pos(v_pos);
		neighbor.set_cost(current_cost);
		return neighbor;
	}

	return p_sol;
}

solution neighborhoods::i_shift_two( solution& p_sol ) {
	unsigned n = cars.get_n();
	vector< t_vec > vehicles = p_sol.get_vehicles();
	vector< matrix_2d > distances = cars.get_distances();

	vector< pair< unsigned, unsigned> > v_pos(p_sol.get_pos());
	vector< unsigned > route(p_sol.get_route());

	// Evaluating all possible shifts
	unsigned i_shift = 0, j_shift = 0;
	double current_cost = p_sol.get_cost();
	for(unsigned k = 0; k < vehicles.size(); k++)
		for(unsigned i = v_pos[k].first + 1; i < v_pos[k].second - 1; i++)
			for(unsigned j = v_pos[k].first; j < v_pos[k].second; j++) {
				// If i == j + 1 means that I'll try to insert i in its same position as before
				if(i == j || i == (j + 1) || i == (j - 1)) continue;

				// Aux variable to calculate the last edge of the cycle
				unsigned aux1 = i + 2;
				if(i == (n - 2)) aux1 = 0;
				unsigned aux2 = j + 1;
				if(j == (n - 1)) aux2 = 0;

				// Calculating the current cost for the or-opt-2 change
				double cost = p_sol.get_cost();
				cost -= distances[ vehicles[k].number ][ route[i - 1] ][ route[i] ];
				cost -= distances[ vehicles[k].number ][ route[i + 1] ][ route[aux1] ];
				cost -= distances[ vehicles[k].number ][ route[j] ][ route[aux2] ];
				cost += distances[ vehicles[k].number ][ route[j] ][ route[i] ];
				cost += distances[ vehicles[k].number ][ route[i + 1] ][ route[aux2] ];
				cost += distances[ vehicles[k].number ][ route[i - 1] ][ route[aux1] ];

				// If the cost is smaller than the current, the change is applied
				if(cost < current_cost) {
					i_shift = i;
					j_shift = j;
					current_cost = cost;
				}
			}

	if(i_shift != j_shift) {
		if(logs)
			cout << "Applying i_shift_2" << endl;
		if(i_shift > j_shift) {
			unsigned values[] = { route[i_shift], route[i_shift + 1] };
			route.erase(route.begin() + i_shift, route.begin() + (i_shift + 2));
			route.insert(route.begin() + (j_shift + 1), values, values + 2);
		} else {
			route.insert(route.begin() + (j_shift + 1), route.begin() + i_shift, route.begin() + (i_shift + 2));
			route.erase(route.begin() + i_shift, route.begin() + (i_shift + 2));
		}
		solution neighbor(cars);
		neighbor.set_route(route);
		neighbor.set_vehicles(vehicles);
		neighbor.set_pos(v_pos);
		neighbor.set_cost(current_cost);
		return neighbor;
	}

	return p_sol;
}

solution neighborhoods::i_shift_three( solution& p_sol ) {
	unsigned n = cars.get_n();
	vector< t_vec > vehicles = p_sol.get_vehicles();
	vector< matrix_2d > distances = cars.get_distances();

	vector< pair< unsigned, unsigned> > v_pos(p_sol.get_pos());
	vector< unsigned > route(p_sol.get_route());

	// Evaluating all possible shifts
	unsigned i_shift = 0, j_shift = 0;
	double current_cost = p_sol.get_cost();
	for(unsigned k = 0; k < vehicles.size(); k++)
		for(int i = v_pos[k].first + 1; i < (int)v_pos[k].second - 2; i++)
			for(unsigned j = v_pos[k].first; j < v_pos[k].second; j++) {
				// If i == j + 1 means that I'll try to insert i in its same position as before
				if(i == j || i == (j + 1) || i == (j - 1) || i == (j - 2)) continue;

				// Aux variable to calculate the last edge of the cycle
				unsigned aux1 = i + 3;
				if(i == (n - 3)) aux1 = 0;
				unsigned aux2 = j + 1;
				if(j == (n - 1)) aux2 = 0;

				// Calculating the current cost for the or-opt-3 change
				double cost = p_sol.get_cost();
				cost -= distances[ vehicles[k].number ][ route[i - 1] ][ route[i] ];
				cost -= distances[ vehicles[k].number ][ route[i + 2] ][ route[aux1] ];
				cost -= distances[ vehicles[k].number ][ route[j] ][ route[aux2] ];
				cost += distances[ vehicles[k].number ][ route[j] ][ route[i] ];
				cost += distances[ vehicles[k].number ][ route[i + 2] ][ route[aux2] ];
				cost += distances[ vehicles[k].number ][ route[i - 1] ][ route[aux1] ];

				// If the cost is smaller than the current, the change is applied
				if(cost < current_cost) {
					i_shift = i;
					j_shift = j;
					current_cost = cost;
				}
			}

	if(i_shift != j_shift) {
		if(logs)
			cout << "Applying i_shift_3" << endl;
		if(i_shift > j_shift) {
			unsigned values[] = { route[i_shift], route[i_shift + 1], route[i_shift + 2] };
			route.erase(route.begin() + i_shift, route.begin() + (i_shift + 3));
			route.insert(route.begin() + (j_shift + 1), values, values + 3);
		} else {
			route.insert(route.begin() + (j_shift + 1), route.begin() + i_shift, route.begin() + (i_shift + 3));
			route.erase(route.begin() + i_shift, route.begin() + (i_shift + 3));
		}
		solution neighbor(cars);
		neighbor.set_route(route);
		neighbor.set_vehicles(vehicles);
		neighbor.set_pos(v_pos);
		neighbor.set_cost(current_cost);
		return neighbor;
	}

	return p_sol;
}

solution neighborhoods::o_swap_one( solution& p_sol ) {
	unsigned n = cars.get_n();
	vector< t_vec > vehicles = p_sol.get_vehicles();
	vector< matrix_2d > distances = cars.get_distances();

	vector< pair< unsigned, unsigned> > v_pos = p_sol.get_pos();
	vector< unsigned > route(p_sol.get_route());

	// Evaluating all possible swaps
	unsigned i_swap = 0, j_swap = 0;
	double current_cost = p_sol.get_cost();
	for(unsigned k = 0; k < vehicles.size() - 1; k++)
		for(unsigned i = v_pos[k].first + 1; i < v_pos[k].second; i++)
			for(unsigned l = k + 1; l < vehicles.size(); l++)
				for(unsigned j = v_pos[l].first + 1; j < v_pos[l].second; j++) {
					// Aux variable to calculate the last edge of the cycle
					unsigned aux = j + 1;
					if(j == (n - 1)) aux = 0;

					// Calculating the current cost for the swap change
					double cost = p_sol.get_cost();
					cost += distances[ vehicles[k].number ][ route[i - 1] ][ route[j] ];
					cost += distances[ vehicles[l].number ][ route[j - 1] ][ route[i] ];
					cost += distances[ vehicles[k].number ][ route[j] ][ route[i + 1] ];
					cost += distances[ vehicles[l].number ][ route[i] ][ route[aux] ];
					cost -= distances[ vehicles[k].number ][ route[i - 1] ][ route[i] ];
					cost -= distances[ vehicles[l].number ][ route[j - 1] ][ route[j] ];
					cost -= distances[ vehicles[k].number ][ route[i] ][ route[i + 1] ];
					cost -= distances[ vehicles[l].number ][ route[j] ][ route[aux] ];

					// If the cost is smaller than the current, the change is applied
					// cout << cost << " - " << neighbor.evaluate() << " = " << cost - neighbor.evaluate() << endl;
					if(cost < current_cost) {
						i_swap = i;
						j_swap = j;
						current_cost = cost;
					}
					// cout << "... executed!" << endl;
				}

	if(i_swap != j_swap) {
		if(logs)
			cout << "Applying o_swap_1\n";
		swap(route[i_swap], route[j_swap]);
		solution neighbor(cars);
		neighbor.set_route(route);
		neighbor.set_vehicles(vehicles);
		neighbor.set_pos(v_pos);
		neighbor.set_cost(current_cost);
		return neighbor;
	}

	return p_sol;
}

solution neighborhoods::o_swap_two( solution& p_sol ) {
	unsigned n = cars.get_n();
	vector< t_vec > vehicles = p_sol.get_vehicles();
	vector< matrix_2d > distances = cars.get_distances();

	vector< pair< unsigned, unsigned> > v_pos = p_sol.get_pos();
	vector< unsigned > route(p_sol.get_route());

	// Evaluating all possible swaps
	unsigned i_swap = 0, j_swap = 0;
	double current_cost = p_sol.get_cost();
	for(unsigned k = 0; k < vehicles.size() - 1; k++)
		for(unsigned i = v_pos[k].first + 1; i < v_pos[k].second - 1; i++)
			for(unsigned l = k + 1; l < vehicles.size(); l++)
				for(unsigned j = v_pos[l].first + 1; j < v_pos[l].second - 1; j++) {
					// Aux variable to calculate the last edge of the cycle
					unsigned aux = j + 2;
					if(j == (n - 2)) aux = 0;

					// Calculating the current cost for the swap change
					double cost = p_sol.get_cost();
					cost += distances[ vehicles[k].number ][ route[i - 1] ][ route[j] ];
					cost += distances[ vehicles[l].number ][ route[j - 1] ][ route[i] ];
					cost += distances[ vehicles[k].number ][ route[j + 1] ][ route[i + 2] ];
					cost += distances[ vehicles[l].number ][ route[i + 1] ][ route[aux] ];
					cost += distances[ vehicles[l].number ][ route[i] ][ route[i + 1] ];
					cost += distances[ vehicles[k].number ][ route[j] ][ route[j + 1] ];
					cost -= distances[ vehicles[k].number ][ route[i - 1] ][ route[i] ];
					cost -= distances[ vehicles[l].number ][ route[j - 1] ][ route[j] ];
					cost -= distances[ vehicles[k].number ][ route[i + 1] ][ route[i + 2] ];
					cost -= distances[ vehicles[l].number ][ route[j + 1] ][ route[aux] ];
					cost -= distances[ vehicles[k].number ][ route[i] ][ route[i + 1] ];
					cost -= distances[ vehicles[l].number ][ route[j] ][ route[j + 1] ];

					// If the cost is smaller than the current, the change is applied
					// cout << cost << " - " << neighbor.evaluate() << " = " << cost - neighbor.evaluate() << endl;
					if(cost < current_cost) {
						i_swap = i;
						j_swap = j;
						current_cost = cost;
					}
					// cout << "... executed!" << endl;
				}

	if(i_swap != j_swap) {
		if(logs)
			cout << "Applying o_swap_2" << endl;
		swap(route[i_swap], route[j_swap]);
		swap(route[i_swap + 1], route[j_swap + 1]);
		solution neighbor(cars);
		neighbor.set_route(route);
		neighbor.set_vehicles(vehicles);
		neighbor.set_pos(v_pos);
		neighbor.set_cost(current_cost);
		return neighbor;
	}

	return p_sol;
}

solution neighborhoods::o_swap_two_one( solution& p_sol ) {
	unsigned n = cars.get_n();
	vector< t_vec > vehicles = p_sol.get_vehicles();
	vector< matrix_2d > distances = cars.get_distances();

	vector< pair< unsigned, unsigned> > v_pos(p_sol.get_pos());
	vector< unsigned > route(p_sol.get_route());

	// Evaluating all possible swaps
	unsigned i_swap = 0, j_swap = 0, k_swap = 0, l_swap = 0;
	double current_cost = p_sol.get_cost();
	for(unsigned k = 0; k < vehicles.size(); k++)
		for(unsigned i = v_pos[k].first + 1; i < v_pos[k].second - 1; i++)
			for(unsigned l = 0; l < vehicles.size(); l++) {
				if(k == l) continue;
				for(unsigned j = v_pos[l].first + 1; j < v_pos[l].second; j++) {
					// Aux variable to calculate the last edge of the cycle
					unsigned aux1 = i + 2;
					if(i == (n - 2)) aux1 = 0;
					unsigned aux2 = j + 1;
					if(j == (n - 1)) aux2 = 0;

					// Calculating the current cost for the swap change
					double cost = p_sol.get_cost();
					cost += distances[ vehicles[k].number ][ route[i - 1] ][ route[j] ];
					cost += distances[ vehicles[l].number ][ route[j - 1] ][ route[i] ];
					cost += distances[ vehicles[k].number ][ route[j] ][ route[aux1] ];
					cost += distances[ vehicles[l].number ][ route[i + 1] ][ route[aux2] ];
					cost += distances[ vehicles[l].number ][ route[i] ][ route[i + 1] ];
					cost -= distances[ vehicles[k].number ][ route[i - 1] ][ route[i] ];
					cost -= distances[ vehicles[l].number ][ route[j - 1] ][ route[j] ];
					cost -= distances[ vehicles[k].number ][ route[i + 1] ][ route[aux1] ];
					cost -= distances[ vehicles[k].number ][ route[i] ][ route[i + 1] ];
					cost -= distances[ vehicles[l].number ][ route[j] ][ route[aux2] ];

					// If the cost is smaller than the current, the change is applied
					// cout << cost << " - " << neighbor.evaluate() << " = " << cost - neighbor.evaluate() << endl;
					if(cost < current_cost) {
						i_swap = i;
						j_swap = j;
						k_swap = k;
						l_swap = l;
						current_cost = cost;
					}
					// cout << "... executed!" << endl;
				}
			}

	if(i_swap != j_swap) {
		if(logs)
			cout << "Applying o_swap_2_1" << endl;
		swap(route[i_swap], route[j_swap]);

		// Shifting i_swap + 1 to its position on the new trip
		unsigned value = route[i_swap + 1];
		if(k_swap > l_swap) {
			route.erase(route.begin() + (i_swap + 1));
			route.insert(route.begin() + (j_swap + 1), value);

			// Updating the position of the intermediate routes
			for(unsigned k = l_swap + 1; k < k_swap; k++) {
				v_pos[k].first++;
				v_pos[k].second++;
			}
			v_pos[k_swap].first++;
			v_pos[l_swap].second++;
		} else {
			route.insert(route.begin() + (j_swap + 1), value);
			route.erase(route.begin() + (i_swap + 1));

			// Updating the position of the intermediate routes
			for(unsigned k = k_swap + 1; k < l_swap; k++) {
				v_pos[k].first--;
				v_pos[k].second--;
			}
			v_pos[k_swap].second--;
			v_pos[l_swap].first--;
		}

		solution neighbor(cars);
		neighbor.set_route(route);
		neighbor.set_vehicles(vehicles);
		neighbor.set_pos(v_pos);
		neighbor.set_cost(current_cost);
		return neighbor;
	}

	return p_sol;
}

solution neighborhoods::o_swap_three( solution& p_sol ) {
	unsigned n = cars.get_n();
	vector< t_vec > vehicles = p_sol.get_vehicles();
	vector< matrix_2d > distances = cars.get_distances();

	vector< pair< unsigned, unsigned> > v_pos = p_sol.get_pos();
	vector< unsigned > route(p_sol.get_route());

	// Evaluating all possible swaps
	unsigned i_swap = 0, j_swap = 0;
	double current_cost = p_sol.get_cost();
	for(unsigned k = 0; k < vehicles.size() - 1; k++) {
		for(int i = v_pos[k].first + 1; i < (int)v_pos[k].second - 2; i++) {
			for(unsigned l = k + 1; l < vehicles.size(); l++) {
				for(int j = v_pos[l].first + 1; j < (int)v_pos[l].second - 2; j++) {
					// Aux variable to calculate the last edge of the cycle
					unsigned aux = j + 3;
					if(j == (n - 3)) aux = 0;

					// Calculating the current cost for the swap change
					double cost = p_sol.get_cost();
					cost += distances[ vehicles[l].number ][ route[j - 1] ][ route[i] ];
					cost += distances[ vehicles[l].number ][ route[i] ][ route[i + 1] ];
					cost += distances[ vehicles[l].number ][ route[i + 1] ][ route[i + 2] ];
					cost += distances[ vehicles[l].number ][ route[i + 2] ][ route[aux] ];
					cost += distances[ vehicles[k].number ][ route[i - 1] ][ route[j] ];
					cost += distances[ vehicles[k].number ][ route[j] ][ route[j + 1] ];
					cost += distances[ vehicles[k].number ][ route[j + 1] ][ route[j + 2] ];
					cost += distances[ vehicles[k].number ][ route[j + 2] ][ route[i + 3] ];
					cost -= distances[ vehicles[k].number ][ route[i - 1] ][ route[i] ];
					cost -= distances[ vehicles[k].number ][ route[i] ][ route[i + 1] ];
					cost -= distances[ vehicles[k].number ][ route[i + 1] ][ route[i + 2] ];
					cost -= distances[ vehicles[k].number ][ route[i + 2] ][ route[i + 3] ];
					cost -= distances[ vehicles[l].number ][ route[j - 1] ][ route[j] ];
					cost -= distances[ vehicles[l].number ][ route[j] ][ route[j + 1] ];
					cost -= distances[ vehicles[l].number ][ route[j + 1] ][ route[j + 2] ];
					cost -= distances[ vehicles[l].number ][ route[j + 2] ][ route[aux] ];

					// If the cost is smaller than the current, the change is applied
					// cout << cost << " - " << neighbor.evaluate() << " = " << cost - neighbor.evaluate() << endl;
					if(cost < current_cost) {
						i_swap = i;
						j_swap = j;
						current_cost = cost;
					}
					// cout << "... executed!" << endl;
				}
			}
		}
	}

	if(i_swap != j_swap) {
		if(logs)
			cout << "Applying o_swap_3" << endl;
		swap(route[i_swap], route[j_swap]);
		swap(route[i_swap + 1], route[j_swap + 1]);
		swap(route[i_swap + 2], route[j_swap + 2]);
		solution neighbor(cars);
		neighbor.set_route(route);
		neighbor.set_vehicles(vehicles);
		neighbor.set_pos(v_pos);
		neighbor.set_cost(current_cost);
		return neighbor;
	}

	return p_sol;
}

solution neighborhoods::o_shift_one( solution& p_sol ) {
	unsigned n = cars.get_n();
	vector< matrix_2d > distances = cars.get_distances();

	vector< t_vec > vehicles = p_sol.get_vehicles();
	vector< pair< unsigned, unsigned> > v_pos(p_sol.get_pos());
	vector< unsigned > route(p_sol.get_route());

	// Evaluating all possible shifts
	unsigned i_shift = 0, j_shift = 0, k_shift = 0, l_shift = 0;
	double current_cost = p_sol.get_cost();
	for(unsigned k = 0; k < vehicles.size(); k++)
		for(unsigned i = v_pos[k].first + 1; i < v_pos[k].second; i++)
			for(unsigned l = 0; l < vehicles.size(); l++) {
				if(k == l) continue;
				for(unsigned j = v_pos[l].first; j < v_pos[l].second; j++) {

					// Aux variable to calculate the last edge of the cycle
					unsigned aux1 = i + 1;
					if(i == (n - 1)) aux1 = 0;
					unsigned aux2 = j + 1;
					if(j == (n - 1)) aux2 = 0;

					// Calculating the current cost for the or-opt-1 change
					double cost = p_sol.get_cost();
					cost -= distances[ vehicles[k].number ][ route[i - 1] ][ route[i] ];
					cost -= distances[ vehicles[k].number ][ route[i] ][ route[aux1] ];
					cost -= distances[ vehicles[l].number ][ route[j] ][ route[aux2] ];
					cost += distances[ vehicles[l].number ][ route[j] ][ route[i] ];
					cost += distances[ vehicles[l].number ][ route[i] ][ route[aux2] ];
					cost += distances[ vehicles[k].number ][ route[i - 1] ][ route[aux1] ];

					// If the cost is smaller than the current, the change is applied
					if(cost < current_cost) {
						i_shift = i;
						j_shift = j;
						k_shift = k;
						l_shift = l;
						current_cost = cost;
					}
				}
			}

	if(k_shift != l_shift) {
		if(logs)
			cout << "Applying o_shift_one\n";
		unsigned value = route[i_shift];
		if(k_shift > l_shift) {
			route.erase(route.begin() + i_shift);
			route.insert(route.begin() + (j_shift + 1), value);

			// Updating the position of the intermediate routes
			for(unsigned k = l_shift + 1; k < k_shift; k++) {
				v_pos[k].first++;
				v_pos[k].second++;
			}
			v_pos[k_shift].first++;
			v_pos[l_shift].second++;
		} else {
			route.insert(route.begin() + (j_shift + 1), value);
			route.erase(route.begin() + i_shift);

			// Updating the position of the intermediate routes
			for(unsigned k = k_shift + 1; k < l_shift; k++) {
				v_pos[k].first--;
				v_pos[k].second--;
			}
			v_pos[k_shift].second--;
			v_pos[l_shift].first--;
		}
		solution neighbor(cars);
		neighbor.set_route(route);
		neighbor.set_vehicles(vehicles);
		neighbor.set_pos(v_pos);
		neighbor.set_cost(current_cost);
		return neighbor;
	}

	return p_sol;
}

solution neighborhoods::o_shift_two( solution& p_sol ) {
	unsigned n = cars.get_n();
	vector< matrix_2d > distances = cars.get_distances();

	vector< t_vec > vehicles = p_sol.get_vehicles();
	vector< pair< unsigned, unsigned> > v_pos(p_sol.get_pos());
	vector< unsigned > route(p_sol.get_route());

	// Evaluating all possible shifts
	unsigned i_shift = 0, j_shift = 0, k_shift = 0, l_shift = 0;
	double current_cost = p_sol.get_cost();
	for(unsigned k = 0; k < vehicles.size(); k++)
		for(unsigned i = v_pos[k].first + 1; i < v_pos[k].second - 1; i++)
			for(unsigned l = 0; l < vehicles.size(); l++) {
				if(k == l) continue;
				for(unsigned j = v_pos[l].first; j < v_pos[l].second; j++) {

					// Aux variable to calculate the last edge of the cycle
					unsigned aux1 = i + 2;
					if(i == (n - 2)) aux1 = 0;
					unsigned aux2 = j + 1;
					if(j == (n - 1)) aux2 = 0;

					// Calculating the current cost for the or-opt-1 change
					double cost = p_sol.get_cost();
					cost -= distances[ vehicles[k].number ][ route[i - 1] ][ route[i] ];
					cost -= distances[ vehicles[k].number ][ route[i] ][ route[i + 1] ];
					cost -= distances[ vehicles[k].number ][ route[i + 1] ][ route[aux1] ];
					cost -= distances[ vehicles[l].number ][ route[j] ][ route[aux2] ];
					cost += distances[ vehicles[l].number ][ route[j] ][ route[i] ];
					cost += distances[ vehicles[l].number ][ route[i] ][ route[i + 1] ];
					cost += distances[ vehicles[l].number ][ route[i + 1] ][ route[aux2] ];
					cost += distances[ vehicles[k].number ][ route[i - 1] ][ route[aux1] ];

					// If the cost is smaller than the current, the change is applied
					if(cost < current_cost) {
						i_shift = i;
						j_shift = j;
						k_shift = k;
						l_shift = l;
						current_cost = cost;
					}
				}
			}

	if(k_shift != l_shift) {
		if(logs)
			cout << "Applying o_shift_two" << endl;
		unsigned values[] = { route[i_shift], route[i_shift + 1] };
		if(k_shift > l_shift) {
			route.erase(route.begin() + i_shift, route.begin() + (i_shift + 2));
			route.insert(route.begin() + (j_shift + 1), values, values + 2);

			// Updating the position of the intermediate routes
			for(unsigned k = l_shift + 1; k < k_shift; k++) {
				v_pos[k].first += 2;
				v_pos[k].second += 2;
			}
			v_pos[k_shift].first += 2;
			v_pos[l_shift].second += 2;
		} else {
			route.insert(route.begin() + (j_shift + 1), values, values + 2);
			route.erase(route.begin() + i_shift, route.begin() + (i_shift + 2));

			// Updating the position of the intermediate routes
			for(unsigned k = k_shift + 1; k < l_shift; k++) {
				v_pos[k].first -= 2;
				v_pos[k].second -= 2;
			}
			v_pos[k_shift].second -= 2;
			v_pos[l_shift].first -= 2;
		}
		solution neighbor(cars);
		neighbor.set_route(route);
		neighbor.set_vehicles(vehicles);
		neighbor.set_pos(v_pos);
		neighbor.set_cost(current_cost);
		return neighbor;
	}

	return p_sol;
}

solution neighborhoods::o_shift_three( solution& p_sol ) {
	unsigned n = cars.get_n();
	vector< matrix_2d > distances = cars.get_distances();

	vector< t_vec > vehicles = p_sol.get_vehicles();
	vector< pair< unsigned, unsigned> > v_pos(p_sol.get_pos());
	vector< unsigned > route(p_sol.get_route());

	// Evaluating all possible shifts
	unsigned i_shift = 0, j_shift = 0, k_shift = 0, l_shift = 0;
	double current_cost = p_sol.get_cost();
	for(unsigned k = 0; k < vehicles.size(); k++)
		for(int i = v_pos[k].first + 1; i < (int)v_pos[k].second - 2; i++)
			for(unsigned l = 0; l < vehicles.size(); l++) {
				if(k == l) continue;
				for(unsigned j = v_pos[l].first; j < v_pos[l].second; j++) {

					// Aux variable to calculate the last edge of the cycle
					unsigned aux1 = i + 3;
					if(i == (n - 3)) aux1 = 0;
					unsigned aux2 = j + 1;
					if(j == (n - 1)) aux2 = 0;

					// Calculating the current cost for the or-opt-1 change
					double cost = p_sol.get_cost();
					cost -= distances[ vehicles[k].number ][ route[i - 1] ][ route[i] ];
					cost -= distances[ vehicles[k].number ][ route[i] ][ route[i + 1] ];
					cost -= distances[ vehicles[k].number ][ route[i + 1] ][ route[i + 2] ];
					cost -= distances[ vehicles[k].number ][ route[i + 2] ][ route[aux1] ];
					cost -= distances[ vehicles[l].number ][ route[j] ][ route[aux2] ];
					cost += distances[ vehicles[l].number ][ route[j] ][ route[i] ];
					cost += distances[ vehicles[l].number ][ route[i] ][ route[i + 1] ];
					cost += distances[ vehicles[l].number ][ route[i + 1] ][ route[i + 2] ];
					cost += distances[ vehicles[l].number ][ route[i + 2] ][ route[aux2] ];
					cost += distances[ vehicles[k].number ][ route[i - 1] ][ route[aux1] ];

					// If the cost is smaller than the current, the change is applied
					if(cost < current_cost) {
						i_shift = i;
						j_shift = j;
						k_shift = k;
						l_shift = l;
						current_cost = cost;
					}
				}
			}

	if(k_shift != l_shift) {
		if(logs)
			cout << "Applying o_shift_three" << endl;
		unsigned values[] = { route[i_shift], route[i_shift + 1], route[i_shift + 2] };
		if(k_shift > l_shift) {
			route.erase(route.begin() + i_shift, route.begin() + (i_shift + 3));
			route.insert(route.begin() + (j_shift + 1), values, values + 3);

			// Updating the position of the intermediate routes
			for(unsigned k = l_shift + 1; k < k_shift; k++) {
				v_pos[k].first += 3;
				v_pos[k].second += 3;
			}
			v_pos[k_shift].first += 3;
			v_pos[l_shift].second += 3;
		} else {
			route.insert(route.begin() + (j_shift + 1), values, values + 3);
			route.erase(route.begin() + i_shift, route.begin() + (i_shift + 3));

			// Updating the position of the intermediate routes
			for(unsigned k = k_shift + 1; k < l_shift; k++) {
				v_pos[k].first -= 3;
				v_pos[k].second -= 3;
			}
			v_pos[k_shift].second -= 3;
			v_pos[l_shift].first -= 3;
		}
		solution neighbor(cars);
		neighbor.set_route(route);
		neighbor.set_vehicles(vehicles);
		neighbor.set_pos(v_pos);
		neighbor.set_cost(current_cost);
		return neighbor;
	}

	return p_sol;
}

solution neighborhoods::exchange( solution& p_sol ) {
	vector< t_vec > vehicles(p_sol.get_vehicles());

	if(vehicles.size() <= 1) return p_sol;

	unsigned n = cars.get_n();
	vector< pair< unsigned, unsigned> > v_pos(p_sol.get_pos());
	vector< matrix_2d > distances = cars.get_distances();
	vector< matrix_2d > rates = cars.get_return_rates();


	vector< unsigned > route = p_sol.get_route();

	// Evaluating all possible exchanges
	unsigned k_swap = 0, l_swap = 0;
	double current_cost = p_sol.get_cost();
	for(unsigned k = 0; k < vehicles.size() - 1; k++)
		for(unsigned l = k + 1; l < vehicles.size(); l++) {
			// Aux variable to calculate the last edge of the cycle
			unsigned aux = v_pos[l].second;
			if(aux == n) aux = 0;

			double cost = p_sol.get_cost();

			// Removing the edges & fees from the previous vehicles
			cost -= rates[ vehicles[k].number ][ route [ v_pos[k].first ] ][ route[ v_pos[k].second ] ];
			cost -= rates[ vehicles[l].number ][ route [ v_pos[l].first ] ][ route[aux] ];
			for(unsigned i = v_pos[k].first; i < v_pos[k].second; i++)
				cost -= distances[ vehicles[k].number ][ route[i] ][ route[i + 1] ];
			for(unsigned i = v_pos[l].first; i < v_pos[l].second - 1; i++)
				cost -= distances[ vehicles[l].number ][ route[i] ][ route[i + 1] ];
			cost -= distances[ vehicles[l].number ][ route[ v_pos[l].second - 1 ] ][ route[aux] ];

			// Adding the edges from the possible exchange in vehicles
			cost += rates[ vehicles[l].number ][ route [ v_pos[k].first ] ][ route[ v_pos[k].second ] ];
			cost += rates[ vehicles[k].number ][ route [ v_pos[l].first ] ][ route[aux] ];
			for(unsigned i = v_pos[k].first; i < v_pos[k].second; i++)
				cost += distances[ vehicles[l].number ][ route[i] ][ route[i + 1] ];
			for(unsigned i = v_pos[l].first; i < v_pos[l].second - 1; i++)
				cost += distances[ vehicles[k].number ][ route[i] ][ route[i + 1] ];
			cost += distances[ vehicles[k].number ][ route[ v_pos[l].second - 1 ] ][ route[aux] ];

			// If the cost is smaller than the current, the change is applied
			if(cost < current_cost) {
				k_swap = k;
				l_swap = l;
				current_cost = cost;
			}
		}

	if(k_swap != l_swap) {
		if(logs)
			cout << "Applying exchange" << endl;
		swap(vehicles[k_swap].number, vehicles[l_swap].number);

		solution neighbor(cars);
		neighbor.set_route(route);
		neighbor.set_vehicles(vehicles);
		neighbor.set_pos(v_pos);
		neighbor.set_cost(current_cost);
		return neighbor;
	}

	return p_sol;
}

solution neighborhoods::full_extend_contract( solution& p_sol ) {
	unsigned n = cars.get_n();
	vector< t_vec > vehicles = p_sol.get_vehicles();
	vector< matrix_2d > distances = cars.get_distances();
	vector< matrix_2d > rates = cars.get_return_rates();

	if(vehicles.size() <= 1) return p_sol;

	vector< unsigned > route = p_sol.get_route();

	solution current = p_sol;

	// Evaluating all possible extensions
	for(unsigned k = 0; k < vehicles.size(); k++) {
		// If is the last vehicle, it is not possible to change its return place
		if(k < vehicles.size() - 1) {
			vector< pair< unsigned, unsigned> > v_pos(p_sol.get_pos());
			// Checking for the extension of all vertexes of the next vehicle

			// NOTE The cost can be recalculated based on the previous iterations

			// Aux variable to calculate the return rate of the last vertex
			unsigned aux = v_pos[k + 1].second;
			if(aux == n) aux = 0;

			double cost = p_sol.get_cost();
			unsigned limit = p_sol.get_trip_size(k + 1) - 1;
			for(unsigned i = 0; i < limit; i++) {

				cost -= distances[ vehicles[k + 1].number ][ route[ v_pos[k + 1].first ] ][ route[ v_pos[k + 1].first + 1 ] ];
				cost -= rates[ vehicles[k + 1].number ][ route[ v_pos[k + 1].first ] ][ route[ aux ] ];
				cost -= rates[ vehicles[k].number ][ route[ v_pos[k].first ] ][ route[ v_pos[k].second ] ];
				cost += distances[ vehicles[k].number ][ route[ v_pos[k + 1].first ] ][ route[ v_pos[k + 1].first + 1 ] ];
				cost += rates[ vehicles[k + 1].number ][ route[ v_pos[k + 1].first + 1 ] ][ route[ aux ] ];
				cost += rates[ vehicles[k].number ][ route[ v_pos[k].first ] ][ route[ v_pos[k].second + 1 ] ];

				v_pos[k].second++;
				v_pos[k + 1].first++;

				// cout << "Vehicle " << vehicles[k].number << "(" << i << "): " << cost;

				// If the cost is smaller than the current, the change is applied
				if(cost < current.get_cost()) {
					vector< t_vec > veh(vehicles);
					veh[k].end = route[ v_pos[k].second ];
					veh[k + 1].begin = route[ v_pos[k + 1].first ];

					solution neighbor(cars);
					neighbor.set_route(route);
					neighbor.set_vehicles(veh);
					neighbor.set_pos(v_pos);
					neighbor.set_cost(cost);
					if(logs)
						cout << "Applying extend_contract (1)" << endl;
					// cout << "Vehicle " << vehicles[k].number << "(" << i << "): " << cost << " - " << neighbor.evaluate() << endl;
					current = neighbor;
				}
				// cout << endl;
			}

      if(logs)
			  cout << route[ v_pos[k + 1].first ] << "->" << route[ aux ] << endl;

			// Evaluating the elimination of the vehicle k + 1 from the solution
			cost -= distances[ vehicles[k + 1].number ][ route[ v_pos[k + 1].first ] ][ route[ aux ] ];
			cost -= rates[ vehicles[k + 1].number ][ route[ v_pos[k + 1].first ] ][ route[ aux ] ];
			cost -= rates[ vehicles[k].number ][ route[ v_pos[k].first ] ][ route[ v_pos[k].second ] ];
			cost += distances[ vehicles[k].number ][ route[ v_pos[k + 1].first ] ][ route[ aux ] ];
			cost += rates[ vehicles[k].number ][ route[ v_pos[k].first ] ][ route[ aux ] ];

			// If the cost is smaller than the current, the change is applied
			if(cost < current.get_cost()) {
				vector< t_vec > veh(vehicles);
				v_pos[k].second++;
				veh[k].end = route[ aux ];
				veh.erase(veh.begin() + k + 1);
				v_pos.erase(v_pos.begin() + k + 1);

				solution neighbor(cars);
				neighbor.set_route(route);
				neighbor.set_vehicles(veh);
				neighbor.set_pos(v_pos);
				neighbor.set_cost(cost);
				if(logs)
					cout << "Applying extend_contract (1) -- deletion" << endl;
				// cout << "Vehicle " << vehicles[k].number << "(" << i << "): " << cost << " - " << neighbor.evaluate() << endl;
				current = neighbor;
			}
		}

		// If is the first vehicle, it is not possible to change its rent place
		if(k > 0) {
			vector< pair< unsigned, unsigned> > v_pos(p_sol.get_pos());
			// Checking for the extension of all vertexes of the previous vehicle but one

			// NOTE the cost can be recalculated based on the previous iterations

			// Aux variable to calculate the return rate of the last vertex
			unsigned aux = v_pos[k].second;
			if(aux == n) aux = 0;

			double cost = p_sol.get_cost();
			unsigned limit = p_sol.get_trip_size(k - 1) - 1;
			for(unsigned i = 0; i < limit; i++) {

				cost -= distances[ vehicles[k - 1].number ][ route[ v_pos[k].first - 1 ] ][ route[ v_pos[k].first ] ];
				cost -= rates[ vehicles[k - 1].number ][ route[ v_pos[k - 1].first ] ][ route[ v_pos[k - 1].second ] ];
				cost -= rates[ vehicles[k].number ][ route[ v_pos[k].first ] ][ route[ aux ] ];
				cost += distances[ vehicles[k].number ][ route[ v_pos[k].first - 1 ] ][ route[ v_pos[k].first ] ];
				cost += rates[ vehicles[k - 1].number ][ route[ v_pos[k - 1].first ] ][ route[ v_pos[k - 1].second - 1 ] ];
				cost += rates[ vehicles[k].number ][ route[ v_pos[k].first - 1 ] ][ route[ aux ] ];

				v_pos[k].first--;
				v_pos[k - 1].second--;

				// cout << "Vehicle " << vehicles[k].number << "(" << i << "): " << cost;

				// If the cost is smaller than the current, the change is applied
				if(cost < current.get_cost()) {
					vector< t_vec > veh(vehicles);
					veh[k].begin = route[ v_pos[k].first ];
					veh[k - 1].end = route[ v_pos[k - 1].second ];

					solution neighbor(cars);
					neighbor.set_route(route);
					neighbor.set_vehicles(veh);
					neighbor.set_pos(v_pos);
					neighbor.set_cost(cost);
					if(logs)
						cout << "Applying extend_contract (2)" << endl;
					// cout << "Vehicle " << vehicles[k].number << "(" << i << "): " << cost << " - " << neighbor.evaluate() << endl;
					current = neighbor;
				}
				// cout << endl;
			}

			// Evaluating the elimination of the vehicle k + 1 from the solution
			cost -= distances[ vehicles[k - 1].number ][ route[ v_pos[k - 1].first ] ][ route[ v_pos[k].first ] ];
			cost -= rates[ vehicles[k - 1].number ][ route[ v_pos[k - 1].first ] ][ route[ v_pos[k - 1].second ] ];
			cost -= rates[ vehicles[k].number ][ route[ v_pos[k].first ] ][ route[ aux ] ];
			cost += distances[ vehicles[k].number ][ route[ v_pos[k - 1].first ] ][ route[ v_pos[k].first ] ];
			cost += rates[ vehicles[k].number ][ route[ v_pos[k - 1].first ] ][ route[ aux ] ];

			// If the cost is smaller than the current, the change is applied
			if(cost < current.get_cost()) {
				vector< t_vec > veh(vehicles);
				v_pos[k].first--;
				veh[k].begin = route[ v_pos[k - 1].first ];
				veh.erase(veh.begin() + (k - 1));
				v_pos.erase(v_pos.begin() + (k - 1));

				solution neighbor(cars);
				neighbor.set_route(route);
				neighbor.set_vehicles(veh);
				neighbor.set_pos(v_pos);
				neighbor.set_cost(cost);
				if(logs)
					cout << "Applying extend_contract (2) -- deletion" << endl;
				// cout << "Vehicle " << vehicles[k].number << "(" << i << "): " << cost << " - " << neighbor.evaluate() << endl;
				current = neighbor;
			}
		}
	}

	return current;
}

solution neighborhoods::extend_contract( solution& p_sol ) {
	unsigned n = cars.get_n();
	vector< t_vec > vehicles = p_sol.get_vehicles();
	vector< matrix_2d > distances = cars.get_distances();
	vector< matrix_2d > rates = cars.get_return_rates();

	if(vehicles.size() <= 1) return p_sol;

	vector< unsigned > route = p_sol.get_route();

	solution current = p_sol;

	// Evaluating all possible extensions
	for(unsigned k = 0; k < vehicles.size(); k++) {
		// If is the last vehicle, it is not possible to change its return place
		if(k < vehicles.size() - 1) {
			vector< pair< unsigned, unsigned> > v_pos(p_sol.get_pos());
			// Checking for the extension of all vertexes of the next vehicle but one

			// NOTE the cost can be recalculated based on the previous iterations

			// Aux variable to calculate the return rate of the last vertex
			unsigned aux = v_pos[k + 1].second;
			if(aux == n) aux = 0;

			double cost = p_sol.get_cost();
			unsigned limit = p_sol.get_trip_size(k + 1) - 1;
			for(unsigned i = 0; i < limit; i++) {

				cost -= distances[ vehicles[k + 1].number ][ route[ v_pos[k + 1].first ] ][ route[ v_pos[k + 1].first + 1 ] ];
				cost -= rates[ vehicles[k + 1].number ][ route[ v_pos[k + 1].first ] ][ route[ aux ] ];
				cost -= rates[ vehicles[k].number ][ route[ v_pos[k].first ] ][ route[ v_pos[k].second ] ];
				cost += distances[ vehicles[k].number ][ route[ v_pos[k + 1].first ] ][ route[ v_pos[k + 1].first + 1 ] ];
				cost += rates[ vehicles[k + 1].number ][ route[ v_pos[k + 1].first + 1 ] ][ route[ aux ] ];
				cost += rates[ vehicles[k].number ][ route[ v_pos[k].first ] ][ route[ v_pos[k].second + 1 ] ];

				v_pos[k].second++;
				v_pos[k + 1].first++;

				// cout << "Vehicle " << vehicles[k].number << "(" << i << "): " << cost;

				// If the cost is smaller than the current, the change is applied
				if(cost < current.get_cost()) {
					vector< t_vec > veh(vehicles);
					veh[k].end = route[ v_pos[k].second ];
					veh[k + 1].begin = route[ v_pos[k + 1].first ];

					solution neighbor(cars);
					neighbor.set_route(route);
					neighbor.set_vehicles(veh);
					neighbor.set_pos(v_pos);
					neighbor.set_cost(cost);
					if(logs)
						cout << "Applying extend_contract (1)" << endl;
					// cout << "Vehicle " << vehicles[k].number << "(" << i << "): " << cost << " - " << neighbor.evaluate() << endl;
					current = neighbor;
				}
				// cout << endl;
			}
		}

		// If is the first vehicle, it is not possible to change its rent place
		if(k > 0) {
			vector< pair< unsigned, unsigned> > v_pos(p_sol.get_pos());
			// Checking for the extension of all vertexes of the previous vehicle but one

			// NOTE the cost can be recalculated based on the previous iterations

			// Aux variable to calculate the return rate of the last vertex
			unsigned aux = v_pos[k].second;
			if(aux == n) aux = 0;

			double cost = p_sol.get_cost();
			unsigned limit = p_sol.get_trip_size(k - 1) - 1;
			for(unsigned i = 0; i < limit; i++) {

				cost -= distances[ vehicles[k - 1].number ][ route[ v_pos[k].first - 1 ] ][ route[ v_pos[k].first ] ];
				cost -= rates[ vehicles[k - 1].number ][ route[ v_pos[k - 1].first ] ][ route[ v_pos[k - 1].second ] ];
				cost -= rates[ vehicles[k].number ][ route[ v_pos[k].first ] ][ route[ aux ] ];
				cost += distances[ vehicles[k].number ][ route[ v_pos[k].first - 1 ] ][ route[ v_pos[k].first ] ];
				cost += rates[ vehicles[k - 1].number ][ route[ v_pos[k - 1].first ] ][ route[ v_pos[k - 1].second - 1 ] ];
				cost += rates[ vehicles[k].number ][ route[ v_pos[k].first - 1 ] ][ route[ aux ] ];

				v_pos[k].first--;
				v_pos[k - 1].second--;

				// cout << "Vehicle " << vehicles[k].number << "(" << i << "): " << cost;

				// If the cost is smaller than the current, the change is applied
				if(cost < current.get_cost()) {
					vector< t_vec > veh(vehicles);
					veh[k].begin = route[ v_pos[k].first ];
					veh[k - 1].end = route[ v_pos[k - 1].second ];

					solution neighbor(cars);
					neighbor.set_route(route);
					neighbor.set_vehicles(veh);
					neighbor.set_pos(v_pos);
					neighbor.set_cost(cost);
					if(logs)
						cout << "Applying extend_contract (2)" << endl;
					// cout << "Vehicle " << vehicles[k].number << "(" << i << "): " << cost << " - " << neighbor.evaluate() << endl;
					current = neighbor;
				}
				// cout << endl;
			}
		}
	}

	return current;
}

solution neighborhoods::extend_contract_one( solution& p_sol ) {
	unsigned n = cars.get_n();
	vector< t_vec > vehicles = p_sol.get_vehicles();
	vector< matrix_2d > distances = cars.get_distances();
	vector< matrix_2d > rates = cars.get_return_rates();

	if(vehicles.size() <= 1) return p_sol;

	vector< unsigned > route = p_sol.get_route();

	solution current = p_sol;

	// Evaluating all possible extensions
	for(unsigned k = 0; k < vehicles.size(); k++) {
		// If is the last vehicle, it is not possible to change its return place
		if(k < vehicles.size() - 1) {
			vector< pair< unsigned, unsigned> > v_pos(p_sol.get_pos());
			// The next vehicle must have more than just one edge
			if(((int)v_pos[k + 1].second - v_pos[k + 1].first) > 1) {

				// Aux variable to calculate the return rate of the last vertex
				unsigned aux = v_pos[k + 1].second;
				if(aux == n) aux = 0;

				double cost = p_sol.get_cost();
				cost -= distances[ vehicles[k + 1].number ][ route[ v_pos[k + 1].first ] ][ route[ v_pos[k + 1].first + 1 ] ];
				cost -= rates[ vehicles[k + 1].number ][ route[ v_pos[k + 1].first ] ][ route[ aux ] ];
				cost -= rates[ vehicles[k].number ][ route[ v_pos[k].first ] ][ route[ v_pos[k].second ] ];
				cost += distances[ vehicles[k].number ][ route[ v_pos[k + 1].first ] ][ route[ v_pos[k + 1].first + 1 ] ];
				cost += rates[ vehicles[k + 1].number ][ route[ v_pos[k + 1].first + 1 ] ][ route[ aux ] ];
				cost += rates[ vehicles[k].number ][ route[ v_pos[k].first ] ][ route[ v_pos[k].second + 1 ] ];

				// If the cost is smaller than the current, the change is applied
				if(cost < current.get_cost()) {
					v_pos[k].second++;
					v_pos[k + 1].first++;
					vector< t_vec > veh(vehicles);
					veh[k].end = route[ v_pos[k].second ];
					veh[k + 1].begin = route[ v_pos[k + 1].first ];

					solution neighbor(cars);
					neighbor.set_route(route);
					neighbor.set_vehicles(veh);
					neighbor.set_pos(v_pos);
					neighbor.set_cost(cost);
					cout << "Vehicle " << vehicles[k].number << ": " << cost << " - " << neighbor.evaluate() << endl;
					current = neighbor;
				}
			}
		}

		// If is the first vehicle, it is not possible to change its rent place
		if(k > 0) {
			vector< pair< unsigned, unsigned> > v_pos(p_sol.get_pos());
			// The previous vehicle must have more than just one edge
			if(((int)v_pos[k - 1].second - v_pos[k - 1].first) > 1) {

				// Aux variable to calculate the return rate of the last vertex
				unsigned aux = v_pos[k].second;
				if(aux == n) aux = 0;

				double cost = p_sol.get_cost();
				cost -= distances[ vehicles[k - 1].number ][ route[ v_pos[k].first - 1 ] ][ route[ v_pos[k].first ] ];
				cost -= rates[ vehicles[k - 1].number ][ route[ v_pos[k - 1].first ] ][ route[ v_pos[k - 1].second ] ];
				cost -= rates[ vehicles[k].number ][ route[ v_pos[k].first ] ][ route[ aux ] ];
				cost += distances[ vehicles[k].number ][ route[ v_pos[k].first - 1 ] ][ route[ v_pos[k].first ] ];
				cost += rates[ vehicles[k - 1].number ][ route[ v_pos[k - 1].first ] ][ route[ v_pos[k - 1].second - 1 ] ];
				cost += rates[ vehicles[k].number ][ route[ v_pos[k].first - 1 ] ][ route[ aux ] ];

				// If the cost is smaller than the current, the change is applied
				if(cost < current.get_cost()) {
					v_pos[k].first--;
					v_pos[k - 1].second--;
					vector< t_vec > veh(vehicles);
					veh[k].begin = route[ v_pos[k].first ];
					veh[k - 1].end = route[ v_pos[k - 1].second ];

					solution neighbor(cars);
					neighbor.set_route(route);
					neighbor.set_vehicles(veh);
					neighbor.set_pos(v_pos);
					neighbor.set_cost(cost);
					cout << "Vehicle " << vehicles[k].number << ": " << cost << " - " << neighbor.evaluate() << endl;
					current = neighbor;
				}
			}
		}

	}

	return current;
}

solution& neighborhoods::execute( solution& p_sol ) {
	if(p_sol.get_vehicles().size() == 1)
		best = inner_RVND(p_sol);
	else
		best = outter_RVND(p_sol);

	return best;
}

solution neighborhoods::inner_RVND( solution& p_sol ) {
	// Setting neighborhood random order
	i_NL = vector< unsigned >(8);
	for(unsigned i = 0; i < 8; i++)
		i_NL[i] = i;
	random_shuffle(i_NL.begin(), i_NL.end(), myrandom);

	// cout << "Executing inner_RVND: ";
	// for(unsigned i = 0; i < 8; i++)
	// 	printf("%d ", i_NL[i]);
	// printf("\n");

	// Executing inner_RVND based on neighborhood order
	solution to_return = p_sol;
	solution aux;
	unsigned i = 0;
	while(i < 8) {
		if(logs)
			printf("    ");
		switch(i_NL[i]) {
			case 0:
				if(logs)
					cout << "Trying i_swap_one...\n";
				aux = i_swap_one(to_return);
				break;
			case 1:
				if(logs)
					cout << "Trying i_swap_two...\n";
				aux = i_swap_two(to_return);
				break;
			case 2:
				if(logs)
					cout << "Trying i_swap_three...\n";
				aux = i_swap_three(to_return);
				break;
			case 3:
				if(logs)
					cout << "Trying two_opt...\n";
				aux = i_two_opt(to_return);
				break;
			case 4:
				if(logs)
					cout << "Trying i_reverse...\n";
				aux = i_reverse(to_return);
				break;
			case 5:
				if(logs)
					cout << "Trying i_shift_one...\n";
				aux = i_shift_one(to_return);
				break;
			case 6:
				if(logs)
					cout << "Trying i_shift_two...\n";
				aux = i_shift_two(to_return);
				break;
			case 7:
				if(logs)
					cout << "Trying i_shift_three...\n";
				aux = i_shift_three(to_return);
				break;
		}

		// If neighborhood find a better solution, the NL is updated
		if(aux.get_cost() < to_return.get_cost()) {
			to_return = aux;
			i = 0;
		} else i++;
	}

	return to_return;
}

solution neighborhoods::outter_RVND( solution& p_sol ) {
	// Setting neighborhood random order
	o_NL = vector< unsigned >(9);
	for(unsigned i = 0; i < 9; i++)
		o_NL[i] = i;
	random_shuffle(o_NL.begin(), o_NL.end(), myrandom);

	// cout << "Executing outter_RVND: ";
	// for(unsigned i = 0; i < 9; i++)
	// 	printf("%d ", o_NL[i]);
	// printf("\n");

	// Executing outter_RVND based on neighborhood order
	solution to_return = p_sol;
	solution aux;
	unsigned i = 0;
	while(i < 9) {
		if(logs)
			printf("  ");
		switch(o_NL[i]) {
			case 0:
				if(logs)
					cout << "Trying o_swap_one...\n";
				aux = o_swap_one(to_return);
				break;
			case 1:
				if(logs)
					cout << "Trying o_swap_two...\n";
				aux = o_swap_two(to_return);
				break;
			case 2:
				if(logs)
					cout << "Trying o_swap_two_one...\n";
				aux = o_swap_two_one(to_return);
				break;
			case 3:
				if(logs)
					cout << "Trying o_swap_three...\n";
				aux = o_swap_three(to_return);
				break;
			case 4:
				if(logs)
					cout << "Trying o_shift_one...\n";
				aux = o_shift_one(to_return);
				break;
			case 5:
				if(logs)
					cout << "Trying o_shift_two...\n";
				aux = o_shift_two(to_return);
				break;
			case 6:
				if(logs)
					cout << "Trying o_shift_three...\n";
				aux = o_shift_three(to_return);
				break;
			case 7:
				if(logs)
					cout << "Trying exchange...\n";
				aux = exchange(to_return);
				break;
			// case 8:
			// 	if(logs)
			// 		cout << "Trying extend_contract...\n";
			// 	aux = extend_contract(to_return);
			// 	break;
			case 8:
				if(logs)
					cout << "Trying full_extend_contract...\n";
				aux = full_extend_contract(to_return);
				break;
		}

		// If neighborhood find a better solution, the NL is updated
		if(aux.get_cost() < to_return.get_cost()) {
			to_return = aux;
			to_return = inner_RVND(to_return);
			i = 0;
		} else i++;
	}

	return to_return;
}
