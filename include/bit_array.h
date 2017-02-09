/*
 * bit_array.h
 *
 *  Created on: Nov 25, 2016
 *      Author: hbarbalho
 */

#ifndef BIT_ARRAY_H_
#define BIT_ARRAY_H_
#include <iostream>
#include <stdint.h>
#include <math.h>

typedef uint32_t number_type;
typedef std::pair<std::size_t,std::size_t> coord;

class BitArray {
private:
	std::size_t n_elements;
	std::size_t bit_array_size;
	number_type *array;
	void init();
	coord get_item_coord(size_t i) const;
public:
	BitArray(size_t _n_elements):n_elements(_n_elements){
		init();
	}
	bool is_in_array(size_t i) const;
	void add(size_t i);
	void remove(size_t i);
	void union_set(const BitArray *b_array);
	bool has_commom_element(const BitArray* const b_array);
	BitArray* get_complement(const BitArray* const b_array) const;
	virtual ~BitArray();
};

#endif /* BIT_ARRAY_H_ */
