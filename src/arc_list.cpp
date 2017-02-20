#include "../include/arc_list.h"

ArcList::ArcList() {
	// TODO Auto-generated constructor stub

}

typedef struct _arc_list_item_cmp{
	bool operator()(_Arc* const a,_Arc* const b) const{
		if(a->get_cost()<b->get_cost())
			return true;
		else if(a->get_cost()>b->get_cost())
			return false;
		else
			return *a < *b;
	}
}arc_list_item_cmp;
const double *values;
typedef struct _array_arc_list_item_cmp{
	bool operator()(_Arc* const a,_Arc* const b) const{
		int v1=values[a->get_var_id()]*1E6;
		int v2=values[b->get_var_id()]*1E6;
		if(v1<v2)
			return true;
		else if(v1>v2)
			return false;
		else
			return *a < *b;
	}
}array_arc_list_item_cmp;


void ArcList::sort(){
	std::sort(graph.begin(),graph.end(),arc_list_item_cmp());
}

void ArcList::sort_by_lp_sol(const double * const lp_sol){
	//array_arc_list_item_cmp::values=_values;
	values=lp_sol;
	std::sort(graph.begin(),graph.end(),array_arc_list_item_cmp());
}

ArcList::~ArcList() {
	// TODO Auto-generated destructor stub
}
