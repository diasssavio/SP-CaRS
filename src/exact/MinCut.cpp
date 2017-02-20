#include "../../include/exact/MinCut.h"
using namespace std;
using namespace lemon;

MinCut::MinCut()
{
	g = NULL;
	alg = NULL;
	dist = NULL;
	cut = NULL;
}

MinCut::~MinCut()
{
	delete (g);
	delete (alg);
	delete (dist);
	delete (cut);
}

void MinCut::init()
{
	if (g != NULL)
		delete g;
	if (alg != NULL)
		delete alg;
	if (dist != NULL)
		delete dist;
	this->N = 0;
	this->g = new Digraph();
	this->dist = new Digraph::ArcMap<double>(*g);
}

void MinCut::build_graph(int N, ArcList::iterator begin, ArcList::iterator end, IloNumMatrix& sol)
{

	for (int i = 0; i < N; ++i)
	{
		addNode();
	}

	for(ArcList::iterator it=begin; it!=end; ++it){
		Arc * arc = *it;
		if(sol[arc->get_i()][arc->get_j()]>1E-6){
			int id = addArc(arc->get_i(), arc->get_j());
			(*dist)[g->arcFromId(id)] = sol[arc->get_i()][arc->get_j()];
		}
	}

}


void MinCut::run_maxflow(int N, ArcList::iterator begin, ArcList::iterator end, IloNumMatrix& sol)
{
	init();

	build_graph(N, begin, end, sol);

	run();
}


int MinCut::addNode()
{
	Digraph::Node node = g->addNode();
	++N;
	return g->id(node);
}

int MinCut::addArc(int i, int j)
{
	Digraph::Node node_i = g->nodeFromId(i);
	Digraph::Node node_j = g->nodeFromId(j);
	Digraph::Arc e = g->addArc(node_i, node_j);
	return g->id(e);
}

void MinCut::addCost(int e_id, double cost)
{
	Digraph::Arc e = g->arcFromId(e_id);
	(*dist)[e] = cost;
}

bool MinCut::is_node_in_cut(int n)
{
	Digraph::Node node = g->nodeFromId(n);
	return (*cut)[node];
}

double MinCut::generate_min_cut(int source, int target)
{
	if(cut!=NULL)
		delete cut;
	cut = new Digraph::NodeMap<bool>(*g);
	if (alg != NULL)
		delete alg;
	this->alg = new EdmondsKarp<Digraph,Digraph::ArcMap<double> >(*g,*dist,g->nodeFromId(source),g->nodeFromId(target));
	this->alg->run();
	double val = this->alg->flowValue();
	this->alg->minCutMap(*cut);
	return val;
}

void MinCut::run()
{

}
