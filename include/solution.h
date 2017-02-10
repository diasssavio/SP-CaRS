//============================================================================
// Name        : solution.h
// Author      : Sávio S. Dias
// E-mail      : diasssavio@gmail.com
// Institution : Universidade Federal Fluminense
// Description : Solution represetation class interface
//============================================================================

#ifndef SOLUTION_H_
#define SOLUTION_H_

#include <vector>
#include <set>
#include <algorithm>
#include <utility>
#include <limits>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <sstream>
#include <fstream>

#include "instance.h"
#include "bit_array.h"
#include "typedef.hpp"

using namespace std;

typedef struct vehicle {
	unsigned number;
	unsigned begin, end;
} t_vec;

class trip {
private:
	vector< unsigned > vertices;
	unsigned renting, returning;

  unsigned n;
  BitArray* bit_vertices;

	double trip_number;

public:
	// Constructors & destructor
	trip();
	trip(vector< unsigned >&, unsigned, unsigned, unsigned);
	virtual ~trip();

	// Getters methods
	const vector< unsigned >& get_vertices() const { return vertices; }
	unsigned get_renting() const { return renting; }
	unsigned get_returning() const { return returning; }
  BitArray* get_bit_vertices() const { return bit_vertices; }
	double get_trip_number() const { return trip_number; }

	// Operators overload
	bool operator==(const trip&);

	// Useful methods
	void show_data();

};

class solution {
private:
	// Instance data class
	instance cars;

	// Solution itself
	vector< unsigned > route;
	vector< t_vec > vehicles;
	vector< pair< unsigned, unsigned> > v_pos;

	// Solution cost
	double cost;

public:
	// Constructors & destructor
	solution();
	solution( instance& );
	solution( solution& );
	virtual ~solution();

	// Setters
	void set_instance( instance& );
	void set_route( const vector< unsigned >& );
	void set_vehicles( const vector< t_vec >& );
	void set_pos( const vector< pair < unsigned, unsigned > >& );
	void set_cost( double );

	// Getters
	instance& get_instance();
	const vector< unsigned >& get_route() const;
	const vector< t_vec >& get_vehicles() const;
	const vector< pair< unsigned, unsigned> >& get_pos() const;
	double get_cost();

	// Useful Methods
	void show_data();
	void show_latex( char*, char* );
	double evaluate();
	void find_pos();
	unsigned get_trip_size( unsigned );
  bool is_used( unsigned );
  vector< unsigned > not_used();
	vector< trip > get_trips();

};

#endif /* SOLUTION_H_ */
