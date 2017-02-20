
#include "../include/arc.h"

_Arc::_Arc(int i, int j, int id,int cost,int supply_cost, int dist) {
	init(i, j, id,cost,supply_cost,dist);
}

_Arc::_Arc(int i, int j) {
	init(i, j, -1,0,0,0);
}

_Arc::~_Arc() {
	// TODO Auto-generated destructor stub
}

void _Arc::init(int _i, int _j, int _id,int _cost,int _supply_cost, int _dist) {
	i = _i;
	j = _j;
	id = _id;
	cost=_cost;
	supply_cost = _supply_cost;
	dist=_dist;
}

bool _Arc::operator==(const _Arc &e) const {
	return (i == e.i && j == e.j) || (i == e.j && j == e.i);
}

size_t _Arc::hash() const {
	size_t hash = 0;
	hash = (i ^ j >> 1) << 1;

	return hash;
}

bool _Arc::operator<(const _Arc& e) const {
	int _i = i;
	int _j = j;
	int _ei = e.i;
	int _ej = e.j;
	if(_i<_ei)
		return true;
	else if(_i>_ei)
		return false;
	else
		return _j<_ej;
}
bool _Arc::operator>(const _Arc& e) const {
	int _i = i;
	int _j = j;
	int _ei = e.i;
	int _ej = e.j;
	if(_i>_ei)
		return true;
	else if(_i<_ei)
		return false;
	else
		return _j>_ej;
}

std::ostream& operator<<(std::ostream &strm, const _Arc& e){
	return strm << "("<< e.i<<","<<e.j<<")";
}
