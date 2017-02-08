//============================================================================
// Name        : constructor.h
// Author      : Sávio S. Dias
// E-mail      : diasssavio@gmail.com
// Institution : Universidade Federal Fluminense
// Description : Solution constructor class interface
//============================================================================

#ifndef CONSTRUCTOR_H_
#define CONSTRUCTOR_H_

#include "../instance.h"
#include "../solution.h"
#include "../mt19937ar.h"

#include <algorithm>
#include <limits>
#include <cmath>

using namespace std;

class constructor {
private:
	instance cars;
	double alpha;
	vector< double > gamma;

public:
	constructor( instance&, double );
	virtual ~constructor();

	// TODO Adapt the constructor to get a solution based on the return_place
	// 			using all available vehicles
	solution generate_sol( vector < unsigned > );

};

#endif /* CONSTRUCTOR_H_ */
