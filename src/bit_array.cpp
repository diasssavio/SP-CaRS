/*
 * bit_array.cpp
 *
 *  Created on: Nov 25, 2016
 *      Author: hbarbalho
 */

#include "../include/bit_array.h"

void BitArray::init(){
	bit_array_size = (size_t)ceil(n_elements/(sizeof(number_type)*8.0));
	array = new number_type[bit_array_size];
	for(size_t t=0;t<bit_array_size;++t)
		array[t]=0;
}

 coord BitArray::get_item_coord(size_t i) const{
	std::size_t pos = (std::size_t)floor(i/(sizeof(number_type)*8.0));
	std::size_t bit = i-pos*sizeof(number_type)*8;
	return std::make_pair(pos,bit);
}

bool BitArray::is_in_array(size_t i) const{
	coord c = get_item_coord(i);
	return array[c.first] & (1<<c.second);
}

void BitArray::add(size_t i){
	coord c = get_item_coord(i);
	array[c.first] = array[c.first] | (1<<c.second);
}

void BitArray::remove(size_t i){
	coord c = get_item_coord(i);
	array[c.first] = array[c.first] & (~(1<<c.second));
}

void BitArray::union_set(const BitArray* const b_array){
	for(size_t t=0;t<bit_array_size;++t)
		this->array[t] = this->array[t] | b_array->array[t];
}

bool BitArray::has_commom_element(const BitArray* const b_array){
	/*std::cout << "Sets: {";
	for(int i=0;i<n_elements;++i)
		if(this->is_in_array(i))
			std::cout << i <<" ";
	std::cout<<"} & {";
	for(int i=0;i<n_elements;++i)
			if(b_array->is_in_array(i))
				std::cout << i <<" ";
	std::cout<<"}"<<std::endl;*/

	number_type has=0;
	for(size_t t=0;t<bit_array_size && has==0;++t)
		has = this->array[t] & b_array->array[t];
	//std::cout << "Has common? "<<has<<std::endl;
	return has>0;
}

BitArray* BitArray::get_complement(const BitArray* const b_array) const{
	BitArray *cmp = new BitArray(n_elements);
	for(size_t t=0;t<bit_array_size;++t)
		cmp->array[t] = ~this->array[t];
}

BitArray::~BitArray() {
	delete[] array;
}
