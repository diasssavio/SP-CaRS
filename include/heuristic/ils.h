//============================================================================
// Name        : ils.h
// Author      : Sávio S. Dias
// E-mail      : diasssavio@gmail.com
// Institution : Universidade Federal Fluminense
// Description : Iterated Local Search class interface
//============================================================================

#ifndef ILS_H_
#define ILS_H_

#include "../instance.h"
#include "../solution.h"
#include "../mt19937ar.h"
#include "constructor.h"
#include "neighborhoods.h"
#include "perturbation.h"
#include "logger.h"

#include <utility>
#include <algorithm>
#include <numeric>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <cmath>

using namespace std;

class ils {
private:
	// Parameters
	unsigned max_it;
	unsigned max_ms_it;
	double alpha;

	// Input instance
	instance cars;

	// Result best solution
	solution best;

	// Logs
	logger* logs;

	// Subsets generation
	int conj[32];
	int cont;
	vector< vector< unsigned > > sets;
	void subsets( unsigned, unsigned );

public:
	ils( instance&, unsigned, unsigned, double, logger* );
	virtual ~ils();

	solution& execute();

};

#endif /* ILS_H_ */
