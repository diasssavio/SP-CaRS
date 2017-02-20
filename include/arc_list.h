#ifndef ARC_LIST_H_
#define ARC_LIST_H_
#include <vector>
#include <algorithm>

#include "arc.h"

using namespace std;

class ArcList {
protected:
	vector<_Arc*> graph;
public:
	typedef vector<_Arc*>::iterator iterator;
	typedef vector<_Arc*>::const_iterator const_iterator;
	ArcList();
	void push_back(_Arc * const _arc){
		graph.push_back(_arc);
	}
	/*Sort by arc cost*/
	void sort();
	/*Sort by linear programming solution value*/
	void sort_by_lp_sol(const double * const lp_sol);
	const _Arc* operator[](size_t i) const{
		return graph[i];
	}
	_Arc* at(size_t i) const{
		return graph[i];
	}
	size_t size(){return graph.size();}
	iterator begin() { return graph.begin(); }
	iterator end() { return graph.end(); }
	virtual ~ArcList();
};

#endif /* ARC_LIST_H_ */
