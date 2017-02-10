//============================================================================
// Name        : solution.cpp
// Author      : Sávio S. Dias
// E-mail      : diasssavio@gmail.com
// Institution : Universidade Federal Fluminense
// Description : Solution represetation class implementation
//============================================================================

#include "../include/solution.h"

trip::trip() {
	renting = returning = 0;
	trip_number = 0.0;
}

trip::trip(vector< unsigned >& _vertices, unsigned _renting, unsigned _returning) : vertices(_vertices), renting(_renting), returning(_returning) {
	trip_number = 0.0;
	int k = 0;
	for(int i = 1; k < vertices.size(); i *= 10)
		trip_number += vertices[k++] * i;
}

trip::~trip() { vertices.clear(); }

bool trip::operator==(const trip& _to_compare) {
	return (_to_compare.get_trip_number() == this->trip_number) && (_to_compare.get_renting() == this->renting) && (_to_compare.get_returning() == this->returning);
}

void trip::show_data() {
	printf("-----------------------------------\n");
	printf("Renting/returning: %d/%d\nTrip: ", renting, returning);
	for(int i = 0; i < vertices.size(); i++)
		printf("%4d", vertices[i]);
	printf("\nNumber representation: %.0lf\n", trip_number);
	printf("-----------------------------------\n");
}

solution::solution() { }

solution::solution( instance& _cars ) {
	cars = _cars;
	route = vector< unsigned >(cars.get_n());
	cost = numeric_limits< double >::max();
}

solution::solution( solution& _sol ) {
	cars = _sol.get_instance();
	route = vector< unsigned >(_sol.get_route());
	vehicles = vector< t_vec >(_sol.get_vehicles());
	v_pos = vector< pair< unsigned, unsigned> >(_sol.get_pos());
	cost = _sol.get_cost();
}

solution::~solution() { }

void solution::set_instance( instance& _cars ) { cars = _cars; }

void solution::set_route( const vector< unsigned >& _route ) { route = _route; }

void solution::set_vehicles( const vector< t_vec >& _vehicles ) { vehicles = _vehicles; }

void solution::set_pos( const vector< pair < unsigned, unsigned > >& _pos ) { v_pos = _pos; }

void solution::set_cost( double _cost ) { cost = _cost; }

instance& solution::get_instance() { return cars; }

const vector< unsigned >& solution::get_route() const { return route; }

const vector< t_vec >& solution::get_vehicles() const { return vehicles; }

const vector< pair< unsigned, unsigned> >& solution::get_pos() const { return v_pos; }

double solution::get_cost() { return cost; }

double solution::evaluate() {
	double cost = 0.0;
	unsigned k = 0;
	for(unsigned i = 0; i < cars.get_n() - 1; i++) {
		cost += cars.get_distances()[ vehicles[k].number ][ route[i] ][ route[i + 1] ];
		// cout << cost << "\t";
		if(vehicles[k].end == route[i + 1])	k++;
	}
	cost += cars.get_distances()[ vehicles[k].number ][ route[cars.get_n() - 1] ][ route[vehicles[k].end] ];
	// cout << cost << "\n";
	// cout << vehicles[k-1].number << "\t" << route[cars.get_n()-1] << "\t" <<
	for(unsigned k = 0; k < vehicles.size(); k++)
		cost += cars.get_return_rates()[ vehicles[k].number ][ vehicles[k].begin ][ vehicles[k].end ];

	return cost;
}

void solution::show_data() {
	printf("-----------------------------------\n");
	printf("VEHICLES:");
	unsigned k = 0;
	for(unsigned i = 0; i < cars.get_n(); i++) {
		printf("%4d ", vehicles[k].number);
		if(vehicles[k].end == route[i + 1]) k++;
	}
	printf("\nROUTE:\t ");
	for(unsigned i = 0; i < cars.get_n(); i++)
		printf("%4d ", route[i]);
	printf("\nCOST: %.2lf\t%.2lf\n", cost, evaluate());
	// printf("\nCOST: %.2lf\n", cost);
	if(cost != evaluate()) {
		cerr << "ERROR: invalid cost value detected!" << endl;
		exit(1);
	}
	printf("-----------------------------------\n");
}

void solution::find_pos() {
	// v_pos = vector< pair< unsigned, unsigned > >(vehicles.size());
	unsigned k = 0;
	unsigned begin = 0;
	for(unsigned i = 0; i < cars.get_n() - 1; i++)
		if(route[begin] == vehicles[k].begin && route[i + 1] == vehicles[k].end) {
			v_pos.push_back(make_pair(begin, i + 1));
			// v_pos[k] = make_pair(begin, i + 1);
			k++;
			begin = i + 1;
		}
	v_pos.push_back(make_pair(begin, cars.get_n()));
	// v_pos[k] = make_pair(begin, 0);
}

void solution::show_latex( char* filename, char* out_file ) {
	ifstream _file;
	_file.open(filename);
	if(_file.is_open()) {
		ofstream _result;
		_result.open(out_file, ios::out);
		if(!_result) return;
		_result << "\\documentclass{standalone}" << endl << "\\usepackage{tikz}" << endl << "\\usetikzlibrary{positioning}" << endl
					 << "\\begin{document}" << endl << "\\begin{tikzpicture}[scale=0.1, every node/.style={scale=0.5,line width=0.5pt}]" << endl;

		char line[256];
		unsigned count = 1;
		bool first = true;
		while(_file.getline(line, 256)) {
			vector< string > values;
			char* block = strtok(line, " ");
			while(block) {
				values.push_back(block);
				block = strtok(NULL, " ");
			}
			if(first) {
				first = false;
				_result << "\\node[draw](" << count << ") at (" << values[1] << "," << values[2] << "){" << count << "};" << endl;
			} else
				_result << "\\node[draw,circle](" << count << ") at (" << values[1] << "," << values[2] << "){" << count << "};" << endl;
			count++;
		}

		unsigned k = 0;
		string colors[5] = {"red", "black", "blue", "green", "yellow"};
		for(unsigned i = 0; i < cars.get_n() - 1; i++) {
			if(vehicles[k].end == route[i]) k++;
			_result << "\\draw[->,line width=1pt," << colors[k] <<"] (" << route[i] + 1 << ") -- (" << route[i+1] + 1 << ") node [midway, fill=white]{" << vehicles[k].number << "};" << endl;
		}
		_result << "\\draw[->,line width=1pt," << colors[k] <<"] (" << route[cars.get_n() - 1] + 1 << ") -- (" << 1 << ") node [midway, fill=white]{" << vehicles[k].number << "};" << endl;

		_result << "\\end{tikzpicture}" << endl << "\\end{document}";
	}
}

unsigned solution::get_trip_size( unsigned c_index ) { return v_pos[c_index].second - v_pos[c_index].first; }

bool solution::is_used( unsigned c_number ) {
  for(unsigned i = 0; i < vehicles.size(); i++)
    if(vehicles[i].number == c_number) return true;
  return false;
}

vector< unsigned > solution::not_used() {
  vector< unsigned > not_used_veh;
  for(unsigned i = 0; i < cars.get_c(); i++)
    if(!is_used(i)) not_used_veh.push_back(i);
  return not_used_veh;
}

vector< trip > solution::get_trips() {
	vector< matrix_2d > distances = cars.get_distances();
	vector< matrix_2d > return_rates = cars.get_return_rates();

	vector< trip > trips;
	vector< unsigned > temp;
	unsigned k = 0;
	for(unsigned i = 0; i < cars.get_n(); i++) {
		temp.push_back(route[i]);
		if(vehicles[k].end == route[i + 1] || (vehicles[k].end == 0 && i == (cars.get_n() - 1)) ){
			trips.push_back(trip(temp, vehicles[k].begin, vehicles[k].end));
			k++;
			temp.clear();
		}
	}

	return trips;
}
