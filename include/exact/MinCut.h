#ifndef MinCut_H_
#define MinCut_H_

#include <lemon/smart_graph.h>
#include <lemon/edmonds_karp.h>
#include <vector>
#include <set>
#include <map>
#include <ilcplex/ilocplex.h>
#include "../arc_list.h"
#include "../arc.h"
#include "../typedef.hpp"

using namespace std;
// using namespace lemon;

typedef IloArray<IloNumVarArray> IloNumVarMatrix;
typedef IloArray<IloNumVarMatrix> IloNumVar3Matrix;
typedef IloArray<IloNumVar3Matrix> IloNumVar4Matrix;

typedef IloArray<IloNumArray> IloNumMatrix;
typedef IloArray<IloNumMatrix> IloNum3Matrix;

class MinCut {
private:
	typedef typename lemon::SmartDigraph Digraph;
	int n;
	Digraph *g;
	// lemon::EdmondsKarp< Digraph, Digraph::ArcMap<double>, Tolerance< double > > *alg;
  lemon::EdmondsKarp< Digraph, Digraph::ArcMap<double> > *alg;
	Digraph::ArcMap<double> *dist;
	Digraph::NodeMap<bool> *cut;
	void init();
	void build_graph(int n, ArcList::iterator begin, ArcList::iterator end, IloNumMatrix& sol);
	void run();

public:
	MinCut();
	void run_maxflow(int n, ArcList::iterator begin, ArcList::iterator end, IloNumMatrix& sol);
	int addNode();
	int addArc(int i, int j);
	void addCost(int e_id, double cost);
	bool is_node_in_cut(int n);
	double generate_min_cut(int source, int target);
	~MinCut();
};

#endif /* MinCut_H_ */
