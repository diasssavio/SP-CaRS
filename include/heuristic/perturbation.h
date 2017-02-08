//============================================================================
// Name        : perturbation.h
// Author      : Sávio S. Dias
// E-mail      : diasssavio@gmail.com
// Institution : Universidade Federal Fluminense
// Description : Perturbation class interface
//============================================================================

#ifndef PERTURBATION_H_
#define PERTURBATION_H_

#include "instance.h"
#include "solution.h"
#include "mt19937ar.h"

#include <utility>
#include <algorithm>
#include <cstdlib>
#include <limits>

class perturbation {
private:
	// Input instance
	instance cars;

public:
	perturbation( instance& );
	virtual ~perturbation();

	solution multiple_shift( solution&, unsigned );
	solution multiple_swap( solution&, unsigned );
  solution outter_vehicle_swap( solution& );
	solution vehicle_swap( solution& );
  solution vehicle_injection( solution& );
	solution double_bridge( solution& );

	solution execute( solution& );

};

#endif /* PERTURBATION_H_ */
