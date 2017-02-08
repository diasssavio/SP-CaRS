//============================================================================
// Name        : instance.h
// Author      : Sávio S. Dias
// E-mail      : diasssavio@gmail.com
// Institution : Universidade Federal Fluminense
// Description : Instance data class interface
//============================================================================

#ifndef INSTANCE_H_
#define INSTANCE_H_

#include <cstdio>
#include <vector>
#include <fstream>

using namespace std;

typedef vector < vector< double > > matrix_2d;

class instance {
private:
	// Primary parameter
	unsigned n; // Number of nodes on the instance
	unsigned c; // Number of cars on the instance

	// General data
	vector< matrix_2d > distances; // Distance matrix to each car
	vector< matrix_2d > return_rates; // Return rate to each car

public:
	// Constructors & Destructors
	instance();
	instance( unsigned, unsigned );
	virtual ~instance();

	// Setters
	void set_n( unsigned );
	void set_c( unsigned );
	void set_distances( vector< matrix_2d >& );
	void set_return_rates( vector< matrix_2d >& );

	// Getters
	unsigned get_n();
	unsigned get_c();
	const vector< matrix_2d >& get_distances() const;
	const vector< matrix_2d >& get_return_rates() const;

	// Useful Methods
	void read_data();
  void read_from_file(const string&);
	void show_data();
};

#endif /* INSTANCE_H_ */
