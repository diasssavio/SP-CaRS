#ifndef ARC_H_
#define ARC_H_
#include <iostream>
#include <algorithm>

class _Arc {
private:
	int i,j;
	/*Arc id*/
	int id;
	/*Solver variable id associated with this Arc*/
	int var_id;
	/*The cost for choose this arc to be in the solution*/
	int cost;
	/*The cost for supply the vertex j with the vertex i*/
	int supply_cost;
	/*The distance from vertex i to vertex j*/
	int dist;
	_Arc *reverse;
	void init(int i,int j,int id,int cost,int supply_cost, int dist);
	friend std::ostream& operator<<(std::ostream&, const _Arc&);
public:
	_Arc(int i, int j, int id,int cost,int supply_cost, int dist);
	_Arc(int i, int j);
	virtual ~_Arc();
	int get_i(){return i;}
	int get_j(){return j;}
	int get_id(){return id;}
	void set_var_id(int _var_id){var_id=_var_id;}
	int get_var_id(){return var_id;}
	int get_cost(){return cost;}
	int get_supply_cost(){return supply_cost;}
	int get_dist(){return dist;}
	_Arc* get_reverse(){return reverse;}
	void set_reverse(_Arc *_reverse){reverse=_reverse;}

	bool operator<(const _Arc& e) const;
	bool operator>(const _Arc& e) const;
	bool operator==(const _Arc &e) const;
	size_t hash()const;
};

#endif /* ARC_H_ */
