//============================================================================
// Name        : instance.cpp
// Author      : Sávio S. Dias
// E-mail      : diasssavio@gmail.com
// Institution : Universidade Federal Fluminense
// Description : Instance data class implementation
//============================================================================

#include "../include/instance.h"

instance::instance() { }

instance::instance( unsigned _n, unsigned _c ) : n(_n), c(_c) {
	distances = vector< matrix_2d >(c);
	return_rates = vector< matrix_2d >(c);
	for(unsigned k = 0; k < c; k++) {
		distances[k] = matrix_2d(n);
		return_rates[k] = matrix_2d(n);
		for(unsigned i = 0; i < n; i++) {
			distances[k][i] = vector< double >(n);
			return_rates[k][i] = vector< double >(n);
		}
	}
}

instance::~instance() { }

void instance::set_n( unsigned _n ) { this->n = _n; }

void instance::set_c( unsigned _c ) { this->c = _c; }

void instance::set_distances( vector< matrix_2d >& _distances) { this->distances = _distances; }

void instance::set_return_rates( vector< matrix_2d >& rates) { this->return_rates = rates; }

unsigned instance::get_n() { return this->n; }

unsigned instance::get_c() { return this->c; }

const vector< matrix_2d >& instance::get_distances() const { return this->distances; }

const vector< matrix_2d >& instance::get_return_rates() const { return this->return_rates; }

void instance::read_data() {
	scanf("%d %d", &n, &c);

	distances = vector< matrix_2d >(c);
	for(unsigned k = 0; k < c; k++) {
		distances[k] = matrix_2d(n);
		for(unsigned i = 0; i < n; i++) {
			distances[k][i] = vector< double >(n);
			for(unsigned j = 0; j < n; j++)
				scanf("%lf", &distances[k][i][j]);
		}
	}

	return_rates = vector< matrix_2d >(c);
	for(unsigned k = 0; k < c; k++) {
		return_rates[k] = matrix_2d(n);
		for(unsigned i = 0; i < n; i++) {
			return_rates[k][i] = vector< double >(n);
			for(unsigned j = 0; j < n; j++)
				scanf("%lf", &return_rates[k][i][j]);
		}
	}
}

void instance::read_from_file(const string& file_name) {
  ifstream _file;
  _file.open(file_name.c_str(), std::ifstream::in);

  if(_file.is_open()) {
    _file >> n;
    _file >> c;

    distances = vector< matrix_2d >(c);
    for(unsigned k = 0; k < c; k++) {
  		distances[k] = matrix_2d(n);
  		for(unsigned i = 0; i < n; i++) {
  			distances[k][i] = vector< double >(n);
  			for(unsigned j = 0; j < n; j++)
          _file >> distances[k][i][j];
      }
    }

    return_rates = vector< matrix_2d >(c);
  	for(unsigned k = 0; k < c; k++) {
  		return_rates[k] = matrix_2d(n);
  		for(unsigned i = 0; i < n; i++) {
  			return_rates[k][i] = vector< double >(n);
  			for(unsigned j = 0; j < n; j++)
  				_file >> return_rates[k][i][j];
  		}
  	}

    _file.close();
  }
}

void instance::show_data() {
	printf("Number of points & Vehicles: %d %d\n\n", n, c);
	printf("Distances matrix:\n");
	for(unsigned k = 0; k < c; k++) {
		for(unsigned i = 0; i < n; i++) {
			for(unsigned j = 0; j < n; j++)
				printf("%.2lf\t", distances[k][i][j]);
			printf("\n");
		}
	}
	printf("Return rates matrix:\n");
	for(unsigned k = 0; k < c; k++) {
		for(unsigned i = 0; i < n; i++) {
			for(unsigned j = 0; j < n; j++)
				printf("%.2lf\t", return_rates[k][i][j]);
			printf("\n");
		}
	}
}
