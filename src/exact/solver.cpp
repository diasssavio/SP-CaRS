//============================================================================
// Name        : solver.cpp
// Author      : Sávio S. Dias
// E-mail      : diasssavio@gmail.com
// Institution : Universidade Federal Fluminense
// Description : Solver class implementation
//============================================================================

#include "../../include/exact/solver.h"

solver::solver(model& _mod) : mod(_mod), IloCplex(_mod) {

}

solver::~solver() { }

void solver::run(double tl, double UB, bool first) {

}
