//============================================================================
// Name        : logger.cpp
// Author      : Sávio S. Dias
// E-mail      : diasssavio@gmail.com
// Institution : Universidade Federal Fluminense
// Description : Logger class implementation
//============================================================================

#include "../include/logger.h"

logger::logger() { }

logger::logger( FWChrono& _timer ) { this->timer = _timer; }

logger::~logger() { }

void logger::make_log( double cost ) {
	it_log.push_back(make_pair(cost, ++iteration));
	times.push_back(((double) timer.getMilliSpan() / 1000));
}

void logger::print_log() {
	printf("MULTI-START ITERATION LOGS:\n");
	for(unsigned i = 0; i < it_log.size(); i++)
		printf("%d: Cost: %.2lf\tTime:%.2lf\n", it_log[i].second, it_log[i].first, times[i]);
}

double logger::best_time() {
  double min_time = 0.0;
	for(unsigned i = 0; i < it_log.size() - 1; i++) {
		if(it_log[i].first == it_log[ it_log.size() - 1 ].first && min_time == 0.0)
			min_time = times[i];
	}

  return min_time;
}
