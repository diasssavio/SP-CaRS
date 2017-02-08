//============================================================================
// Name        : neighborhoods.h
// Author      : Sávio S. Dias
// E-mail      : diasssavio@gmail.com
// Institution : Universidade Federal Fluminense
// Description : Neighborhoods class interface
//============================================================================

#ifndef NEIGHBORHOODS_H_
#define NEIGHBORHOODS_H_

#include "../instance.h"
#include "../solution.h"
#include "../mt19937ar.h"
#include "constructor.h"

#include <utility>
#include <algorithm>
#include <cstdlib>
#include <limits>

using namespace std;

int myrandom( unsigned );

class neighborhoods {
private:
	// Input instance
	instance cars;

	vector< unsigned > i_NL;
	vector< unsigned > o_NL;

	bool logs;

	// Result best solution
	solution best;

public:
	neighborhoods( instance&, bool = false );
	virtual ~neighborhoods();

	// TSP (route) Neighborhoods -- INNER ROUTES
	// TODO Adapt inner route neighborhoods to avoid evaluation of different routes
	//			In this case, one may find the best to each route without reavaluate it later
	// Swap-exchange neighborhoods
	solution i_swap_one( solution& );
	solution i_swap_two( solution& );
	solution i_swap_three( solution& );

	// K-opt & reverse neighborhoods
	solution i_two_opt( solution& );
	solution i_reverse( solution& );

	// Or-Opt (Shift) neighborhoods
	solution i_shift_one( solution& );
	solution i_shift_two( solution& );
	solution i_shift_three( solution& );

	// TSP (route) Neighborhoods -- OUTTER ROUTES
	// Swap-exchange neighborhoods
	solution o_swap_one( solution& );
	solution o_swap_two( solution& );
	solution o_swap_two_one( solution& );
	solution o_swap_three( solution& );

	// Or-Opt (Shift) neighborhoods
	solution o_shift_one( solution& );
	solution o_shift_two( solution& );
	solution o_shift_three( solution& );

	// Vehicles neighborhoods -- Also OUTTER ROUTES
	solution exchange( solution& );
	solution full_extend_contract( solution& );
	solution extend_contract( solution& );

	// ** Legacy method **
	solution extend_contract_one( solution& );

	solution& execute( solution& );

	solution inner_RVND( solution& );
	solution outter_RVND( solution& );

};

#endif /* NEIGHBORHOODS_H_ */
