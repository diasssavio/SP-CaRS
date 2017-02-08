//============================================================================
// Name        : main.cpp
// Author      : Sávio S. Dias
// E-mail      : diasssavio@gmail.com
// Institution : Universidade Federal Fluminense
// Description : Main program file
//============================================================================

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <ilcplex/ilocplex.h>

#include "../include/FWChrono.h"
#include "../include/mt19937ar.h"
#include "../include/instance.h"
#include "../include/solution.h"
#include "../include/heuristic/logger.h"
#include "../include/heuristic/constructor.h"
#include "../include/heuristic/ils.h"

using namespace std;

template<typename T>
T string_to(const string& s){
	istringstream i(s);
	T x;
	if (!(i >> x)) return 0;
	return x;
}

ILOSTLBEGIN

int main(int argc, char* args[]) {
	FWChrono timer;
	timer.start();

	unsigned long seed = time(NULL);
	if(argc >= 3)
		seed = string_to< unsigned long >(args[2]);
	// time_t seed = time(NULL);
	// srand(seed);
	init_genrand(seed);

	instance cars;
	// cars.read_data();
  cars.read_from_file(args[1]);

  double alpha = 0.47;
	unsigned max_it = 61;
	unsigned max_ms_it = 28;

  // double alpha = 0.25;
	// unsigned max_it = 50;
	// unsigned max_ms_it = 20;

  if(argc >= 4) {
    alpha = string_to< double >(args[3]);
    max_it = string_to< unsigned >(args[4]);
    max_ms_it = string_to< unsigned >(args[5]);
  }

	logger* logs = new logger(timer);
	ils ILS(cars, max_ms_it, max_it, alpha, logs);
	solution best = ILS.execute();
	timer.stop();
  printf("BEST %.0lf TIME %.2lf", best.get_cost(), timer.getStopTime());
	// printf("%.2lf;%.2lf;", best.get_cost(), timer.getStopTime());
  // printf("%.2lf;%.2lf;%.2lf;", best.get_cost(), logs->best_time(), timer.getStopTime());
	// printf("BEST FOUND SOLUTION -- %.2lf:\n", timer.getStopTime());
	// best.show_data();

	// if(argc >= 3)
	// 	best.show_latex(args[2], "plot.tex");

	return 0;
}
