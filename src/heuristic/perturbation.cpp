//============================================================================
// Name        : perturbation.cpp
// Author      : Sávio S. Dias
// E-mail      : diasssavio@gmail.com
// Institution : Universidade Federal Fluminense
// Description : Perturbation class implementation
//============================================================================

#include "../include/perturbation.h"

perturbation::perturbation( instance& _cars ) { this->cars = _cars; }

perturbation::~perturbation() { }

solution perturbation::multiple_shift( solution& p_sol, unsigned max_pert ) {
	int number_pert = 1 + (genrand_int32() % max_pert);

	unsigned n = cars.get_n();
	vector< matrix_2d > distances = cars.get_distances();

	vector< t_vec > vehicles = p_sol.get_vehicles();

	if(vehicles.size() <= 1) {
    solution to_ret;
    to_ret = multiple_swap(p_sol, max_pert);
    return to_ret;
  }

	vector< pair< unsigned, unsigned> > v_pos = p_sol.get_pos();
	vector< unsigned > route(p_sol.get_route());

	double cost = p_sol.get_cost();
	while(number_pert > 0) {
		// Raffling the two distinct routes to be perturbed
		unsigned k = genrand_int32() % vehicles.size();
		unsigned l = genrand_int32() % vehicles.size();

		if(k == l) {
			if((l + 1) < vehicles.size()) l++;
			else l--;
		}

		number_pert--;

		// Impossible to shift a route with only the renting places
		if(p_sol.get_trip_size(k) <= 1 || p_sol.get_trip_size(l) <= 1) continue;

		// Raffling the first element to be shifted & its position
		unsigned i_first = v_pos[k].first + 1 + (genrand_int32() % (p_sol.get_trip_size(k) - 1));
		unsigned j_first = v_pos[l].first + (genrand_int32() % p_sol.get_trip_size(l));

		// Aux variable to calculate the last edge of the cycle
		unsigned aux1 = i_first + 1;
		if(i_first == (n - 1)) aux1 = 0;
		unsigned aux2 = j_first + 1;
		if(j_first == (n - 1)) aux2 = 0;

		// Calculating the current cost for the first shift (k~>l)
		cost -= distances[ vehicles[k].number ][ route[i_first - 1] ][ route[i_first] ];
		cost -= distances[ vehicles[k].number ][ route[i_first] ][ route[aux1] ];
		cost -= distances[ vehicles[l].number ][ route[j_first] ][ route[aux2] ];
		cost += distances[ vehicles[l].number ][ route[j_first] ][ route[i_first] ];
		cost += distances[ vehicles[l].number ][ route[i_first] ][ route[aux2] ];
		cost += distances[ vehicles[k].number ][ route[i_first - 1] ][ route[aux1] ];

		// printf("Cost: %.2lf\n", cost);

		// Raffling the second element to be shifted & its position
		unsigned i_second = v_pos[l].first + 1 + (genrand_int32() % (p_sol.get_trip_size(l) - 1));
		unsigned j_second = v_pos[k].first + (genrand_int32() % p_sol.get_trip_size(k));

		// Aux variable to calculate the last edge of the cycle
		unsigned aux3 = i_second + 1;
		if(i_second == (n - 1)) aux3 = 0;
		unsigned aux4 = j_second + 1;
		if(j_second == (n - 1)) aux4 = 0;

		// Calculating the current cost for the second shift (l~>k)
		if(j_second == i_first) {
			cost -= distances[ vehicles[k].number ][ route[j_second - 1] ][ route[aux4] ];
			cost += distances[ vehicles[k].number ][ route[j_second - 1] ][ route[i_second] ];
			cost += distances[ vehicles[k].number ][ route[i_second] ][ route[aux4] ];
		} else if((j_second + 1) == i_first) {
			cost -= distances[ vehicles[k].number ][ route[j_second] ][ route[aux1] ];
			cost += distances[ vehicles[k].number ][ route[j_second] ][ route[i_second] ];
			cost += distances[ vehicles[k].number ][ route[i_second] ][ route[aux1] ];
		} else {
			cost -= distances[ vehicles[k].number ][ route[j_second] ][ route[aux4] ];
			cost += distances[ vehicles[k].number ][ route[j_second] ][ route[i_second] ];
			cost += distances[ vehicles[k].number ][ route[i_second] ][ route[aux4] ];
		}
		if(j_first == i_second) {
			cost -= distances[ vehicles[l].number ][ route[i_second] ][ route[i_first] ];
			cost -= distances[ vehicles[l].number ][ route[i_second - 1] ][ route[i_second] ];
			cost += distances[ vehicles[l].number ][ route[i_second - 1] ][ route[i_first] ];
		} else if((j_first + 1) == i_second) {
			cost -= distances[ vehicles[l].number ][ route[i_first] ][ route[i_second] ];
			cost -= distances[ vehicles[l].number ][ route[i_second] ][ route[aux3] ];
			cost += distances[ vehicles[l].number ][ route[i_first] ][ route[aux3] ];
		} else {
			cost -= distances[ vehicles[l].number ][ route[i_second] ][ route[aux3] ];
			cost -= distances[ vehicles[l].number ][ route[i_second - 1] ][ route[i_second] ];
			cost += distances[ vehicles[l].number ][ route[i_second - 1] ][ route[aux3] ];
		}

		// Applying the changes on the route
		unsigned value1 = route[i_first], value2 = route[i_second];
		// printf("%d<~>%d: %d~>%d \t %d~>%d\n", k, l, i_first, j_first, i_second, j_second);
		if(k > l) {
			route.insert(route.begin() + (j_second + 1), value2);
			route.insert(route.begin() + (j_first + 1), value1);
			if(j_second >= i_first) route.erase(route.begin() + i_first + 1);
			else route.erase(route.begin() + i_first + 2);
		} else {
			route.insert(route.begin() + (j_first + 1), value1);
			route.insert(route.begin() + (j_second + 1), value2);
			if(j_second >= i_first) route.erase(route.begin() + i_first);
			else route.erase(route.begin() + i_first + 1);
		}
		if(j_first >= i_second) route.erase(route.begin() + i_second);
		else route.erase(route.begin() + i_second + 1);
	}

	solution perturbed(cars);
	perturbed.set_route(route);
	perturbed.set_vehicles(vehicles);
	perturbed.set_pos(v_pos);
	perturbed.set_cost(cost);

	return perturbed;
}

solution perturbation::multiple_swap( solution& p_sol, unsigned max_pert ) {
	unsigned number_pert = 1 + (genrand_int32() % max_pert);

	unsigned n = cars.get_n();
	vector< t_vec > vehicles = p_sol.get_vehicles();
	vector< matrix_2d > distances = cars.get_distances();

	vector< pair< unsigned, unsigned> > v_pos = p_sol.get_pos();
	vector< unsigned > route(p_sol.get_route());

	double cost = p_sol.get_cost();
	while(number_pert > 0) {
		// Raffling the two routes to be perturbed
		unsigned k = genrand_int32() % vehicles.size();
		unsigned l = genrand_int32() % vehicles.size();

		number_pert--;

		// Impossible to swap routes with only the renting places
		if(p_sol.get_trip_size(k) <= 1 || p_sol.get_trip_size(l) <= 1) continue;

		// Raffling the elements to be swapped
		unsigned i = v_pos[k].first + 1 + (genrand_int32() % (p_sol.get_trip_size(k) - 1));
		unsigned j = v_pos[l].first + 1 + (genrand_int32() % (p_sol.get_trip_size(l) - 1));

		// Check for swapping equals vertices and avoid inner swap with trip_size = 2
		if(i == j) {
			if((j + 1) < v_pos[l].second) j++;
			else if((j - 1) > v_pos[l].first) j--;
			else continue;
		}
		if(i > j) {
			swap(i, j);
			swap(k, l);
		}

		// printf("%d <-> %d \n", route[i], route[j]);

		// Aux variable to calculate the last edge of the cycle
		unsigned aux1 = i + 1;
		if(i == (n - 1)) aux1 = 0;
		unsigned aux2 = j + 1;
		if(j == (n - 1)) aux2 = 0;

		// Calculating the current cost for the swap change
		cost += distances[ vehicles[k].number ][ route[i - 1] ][ route[j] ];
		cost += distances[ vehicles[l].number ][ route[i] ][ route[aux2] ];
		cost -= distances[ vehicles[k].number ][ route[i - 1] ][ route[i] ];
		cost -= distances[ vehicles[l].number ][ route[j] ][ route[aux2] ];
		cost -= distances[ vehicles[k].number ][ route[i] ][ route[aux1] ];
		if(k == l) { // Inner swap
			// If i & j are adjacent
			if(j == (i + 1) || i == (j + 1))
				cost += distances[ vehicles[k].number ][ route[j] ][ route[i] ];
			else { // otherwise
				cost += distances[ vehicles[k].number ][ route[j] ][ route[aux1] ];
				cost += distances[ vehicles[k].number ][ route[j - 1] ][ route[i] ];
				cost -= distances[ vehicles[k].number ][ route[j - 1] ][ route[j] ];
			}
		} else { // Outter swap
			cost += distances[ vehicles[l].number ][ route[j - 1] ][ route[i] ];
			cost += distances[ vehicles[k].number ][ route[j] ][ route[aux1] ];
			cost -= distances[ vehicles[l].number ][ route[j - 1] ][ route[j] ];
		}

		swap(route[i], route[j]);
	}

	solution perturbed(cars);
	perturbed.set_route(route);
	perturbed.set_vehicles(vehicles);
	perturbed.set_pos(v_pos);
	perturbed.set_cost(cost);

	return perturbed;
}

solution perturbation::outter_vehicle_swap( solution& p_sol ) {
	vector< t_vec > vehicles(p_sol.get_vehicles());
  unsigned c = cars.get_c();

  if(vehicles.size() == c) {
    solution to_ret;
    to_ret = multiple_swap(p_sol, 1);
    return to_ret;
  }

	unsigned n = cars.get_n();
	vector< pair< unsigned, unsigned> > v_pos = p_sol.get_pos();
	vector< matrix_2d > distances = cars.get_distances();
	vector< matrix_2d > rates = cars.get_return_rates();
	vector< unsigned > route = p_sol.get_route();

	// Raffling the two vehicles to be perturbed
	unsigned k = genrand_int32() % vehicles.size();
  vector< unsigned > not_used = p_sol.not_used();
	unsigned l = genrand_int32() % not_used.size();

	// Aux variable to calculate the last edge of the cycle
	unsigned aux = v_pos[k].second;
	if(aux == n) aux = 0;

	double cost = p_sol.get_cost();

	// Removing the edges & fee from the previous vehicle
	cost -= rates[ vehicles[k].number ][ route [ v_pos[k].first ] ][ route[aux] ];
	for(unsigned i = v_pos[k].first; i < v_pos[k].second - 1; i++)
		cost -= distances[ vehicles[k].number ][ route[i] ][ route[i + 1] ];
	cost -= distances[ vehicles[k].number ][ route[ v_pos[k].second - 1 ] ][ route[aux] ];

	// Adding the edges from the possible exchange in vehicles
	cost += rates[ not_used[l] ][ route [ v_pos[k].first ] ][ route[aux] ];
	for(unsigned i = v_pos[k].first; i < v_pos[k].second - 1; i++)
		cost += distances[ not_used[l] ][ route[i] ][ route[i + 1] ];
	cost += distances[ not_used[l] ][ route[ v_pos[k].second - 1 ] ][ route[aux] ];

  vehicles[k].number = not_used[l];

	solution perturbed(cars);
	perturbed.set_route(route);
	perturbed.set_vehicles(vehicles);
	perturbed.set_pos(v_pos);
	perturbed.set_cost(cost);

	return perturbed;
}

solution perturbation::vehicle_swap( solution& p_sol ) {
	vector< t_vec > vehicles(p_sol.get_vehicles());

	if(vehicles.size() <= 1) {
    solution to_ret;
    to_ret = multiple_swap(p_sol, 1);
    return to_ret;
  }

	unsigned n = cars.get_n();
	vector< pair< unsigned, unsigned> > v_pos(p_sol.get_pos());
	vector< matrix_2d > distances = cars.get_distances();
	vector< matrix_2d > rates = cars.get_return_rates();
	vector< unsigned > route = p_sol.get_route();

	// Raffling the two routes to be perturbed
	unsigned k = genrand_int32() % vehicles.size();
	unsigned l = genrand_int32() % vehicles.size();

	if(k == l) {
		if((l + 1) < vehicles.size()) l++;
		else l--;
	}
	if(k > l) swap(k, l);

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

	swap(vehicles[k].number, vehicles[l].number);

	solution perturbed(cars);
	perturbed.set_route(route);
	perturbed.set_vehicles(vehicles);
	perturbed.set_pos(v_pos);
	perturbed.set_cost(cost);

	return perturbed;
}

solution perturbation::vehicle_injection( solution& p_sol ) {
  vector< t_vec > vehicles(p_sol.get_vehicles());
  unsigned c = cars.get_c();

  if(vehicles.size() == c) {
    solution to_ret;
    to_ret = vehicle_swap(p_sol);
    return to_ret;
  }

	unsigned n = cars.get_n();
	vector< pair< unsigned, unsigned> > v_pos(p_sol.get_pos());
	vector< matrix_2d > distances = cars.get_distances();
	vector< matrix_2d > rates = cars.get_return_rates();
	vector< unsigned > route = p_sol.get_route();

	// Raffling the two vehicles to be perturbed
	unsigned k = genrand_int32() % vehicles.size();
  while(p_sol.get_trip_size(k) <= 1) {
    if((k + 1) < vehicles.size()) k++;
		else k--;
  }
  vector< unsigned > not_used = p_sol.not_used();
	unsigned l = genrand_int32() % not_used.size();

  // Raffling number of vertices to be assigned to inserted vehicle and its beginning place
  unsigned n_vertices = 1 + (genrand_int32() % (p_sol.get_trip_size(k) - 1));
  unsigned start_place = genrand_int32() % 2;

	// Aux variable to calculate the last edge of the cycle
	unsigned aux = v_pos[k].second;
	if(aux == n) aux = 0;

	double cost = p_sol.get_cost();

	// Removing the edges & fee from the previous vehicle
	cost -= rates[ vehicles[k].number ][ route[ v_pos[k].first ] ][ route[aux] ];
  if(!start_place) {
  	for(unsigned i = v_pos[k].first; i < v_pos[k].first + n_vertices; i++)
  		cost -= distances[ vehicles[k].number ][ route[i] ][ route[i + 1] ];
  } else {
    for(unsigned i = v_pos[k].first + n_vertices; i < v_pos[k].second - 1; i++)
      cost -= distances[ vehicles[k].number ][ route[i] ][ route[i + 1] ];
    cost -= distances[ vehicles[k].number ][ route[ v_pos[k].second - 1 ] ][ route[aux] ];
  }

	// Adding the edges & fees from the new & previous vehicles
  if(!start_place) {
    cost += rates[ not_used[l] ][ route[ v_pos[k].first ] ][ route[ v_pos[k].first + n_vertices ] ];
    cost += rates[ vehicles[k].number ][ route[ v_pos[k].first + n_vertices ] ][ route[aux] ];
    for(unsigned i = v_pos[k].first; i < v_pos[k].first + n_vertices; i++)
  		cost += distances[ not_used[l] ][ route[i] ][ route[i + 1] ];
  } else {
    cost += rates[ vehicles[k].number ][ route[ v_pos[k].first ] ][ route[ v_pos[k].first + n_vertices ] ];
    cost += rates[ not_used[l] ][ route[ v_pos[k].first + n_vertices ] ][ route[aux] ];
    for(unsigned i = v_pos[k].first + n_vertices; i < v_pos[k].second - 1; i++)
  		cost += distances[ not_used[l] ][ route[i] ][ route[i + 1] ];
  	cost += distances[ not_used[l] ][ route[ v_pos[k].second - 1 ] ][ route[aux] ];
  }

  // Updating solution information
  t_vec temp;
  temp.number = not_used[l];
  if(!start_place) {
    temp.begin = route[ v_pos[k].first ];
    temp.end = route[ v_pos[k].first + n_vertices ];
    vehicles[k].begin = route[ v_pos[k].first + n_vertices ];
    vehicles.insert(vehicles.begin() + k, temp);
  } else {
    temp.begin = route[ v_pos[k].first + n_vertices ];
    temp.end = route[aux];
    vehicles[k].end = route[ v_pos[k].first + n_vertices ];
    vehicles.insert(vehicles.begin() + (k + 1), temp);
  }
  v_pos.insert(v_pos.begin() + (k + 1), make_pair(v_pos[k].first + n_vertices, v_pos[k].second));
  v_pos[k].second = v_pos[k].first + n_vertices;

	solution perturbed(cars);
	perturbed.set_route(route);
	perturbed.set_vehicles(vehicles);
	perturbed.set_pos(v_pos);
	perturbed.set_cost(cost);
  // perturbed.set_cost(perturbed.evaluate());

	return perturbed;
}

solution perturbation::double_bridge( solution& p_sol ) { }

solution perturbation::execute( solution& p_sol ) {
	unsigned perturb = genrand_int32() % 4;
	solution perturbed;
	switch(perturb) {
		case 0:
			// printf("Multiple Shift:\n");
			perturbed = multiple_shift(p_sol, 1);
			break;
		case 1:
			// printf("Multiple Swap:\n");
			perturbed = multiple_swap(p_sol, 1);
			break;
		case 2:
			// printf("Vehicle Swap:\n");
			perturbed = vehicle_swap(p_sol);
			break;
    case 3:
			// printf("Vehicle Injection:\n");
			// perturbed = outter_vehicle_swap(p_sol);
      perturbed = vehicle_injection(p_sol);
			break;
		// case 3:
		// 	printf("Double Bridge:\n");
		// 	perturbed = double_bridge(p_sol);
		// 	break;
	}

	return perturbed;
}
