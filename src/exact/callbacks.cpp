//============================================================================
// Name        : callbacks.cpp
// Author      : Sávio S. Dias
// E-mail      : diasssavio@gmail.com
// Institution : Universidade Federal Fluminense
// Description : Callbacks class implementation
//============================================================================

#include "../../include/exact/callbacks.h"

void LR_callback::main() {
  if(root_node) {
		solve();
		*linear_obj = getObjValue();
		*linear_time = ((double) timer.getMilliSpan() / 1000);

		root_node = false;
	}
}

void mipinfo_callback::main() {
  // Only gather RL information on the root node
  unsigned nodes = getNnodes();
  if(nodes == 0) {
    *linear_obj = getBestObjValue();
    *linear_time = ((double) timer.getMilliSpan() / 1000);
  }
}

void hao_cutsetcallback::main() {
  unsigned c = cars.get_c();
  unsigned n = cars.get_n();

  bool logs = true;
	if(logs)
		_file << "\nEntering hao_cutsetcallback. Solution value:" << getObjValue() << "\n";

  // Building edges list and assigning variables LR values
  vector<str_edge> edges;
  vector<double> weights;
  for(int i = 0; i < n; i++)
    for(int j = i + 1; j < n; j++) {
      str_edge edge;
      edge.v1 = i;
      edge.v2 = j;
      edges.push_back(edge);

      // Getting edges weights ij and ji
      double weight = 0.0;
      for(int k = 0; k < c; k++)
        weight += getValue(x[k][i][j]);
      weights.push_back(weight);
      weight = 0.0;
      for(int k = 0; k < c; k++)
        weight += getValue(x[k][j][i]);
      weights.push_back(weight);
    }

  // if(logs)
	// 	_file << "Neighborhood list built!...\n";

  // Applying minimum cut algorithm
  vector<vector<int> > cuts;
  Gmindcut alg = Gmindcut(_file);
  alg.start(n, edges.size(), edges);
  int ncuts = alg.generate_cuts(weights, cuts, edges, false);
  _file << "Number of cuts found: " << ncuts << endl;

  _file << "Cuts matrix:\n";
  for(unsigned i = 0; i < cuts.size(); i++) {
    for(unsigned j = 0; j < cuts[i].size(); j++)
      _file << cuts[i][j] << "\t";
    _file << "\n";
  }

  // Evaluating whether there's constraint violation
  for(unsigned aux = 0; aux < cuts.size(); aux++) {
    vector<int> not_S;
    for(unsigned i = 0; i < n; i++)
      if(find(cuts[aux].begin(), cuts[aux].end(), i) == cuts[aux].end())
        not_S.push_back(i);

    IloExpr lhs(getEnv());
    for(unsigned i = 0; i < not_S.size(); i++)
      for(unsigned j = 0; j < cuts[aux].size(); j++)
        for(unsigned k = 0; k < c; k++)
          lhs += x[k][ not_S[i] ][ cuts[aux][j] ];

    IloConstraint cut = (lhs >= 1.0);
    cut = add(cut);
    if(logs)
      _file << cut << endl;
    cut.end();
    n_cuts++;
  }

  if(logs)
		_file << "Leaving hao_cutsetcallback\n";
}

void hao_cutsetcallback2::main() {
  unsigned c = cars.get_c();
  unsigned n = cars.get_n();

  bool logs = true;
	if(logs)
		_file << "\nEntering hao_cutsetcallback. Solution value:" << getObjValue() << "\n";

  // Building edges list and assigning variables LR values
  vector<str_edge> edges;
  vector<double> weights;
  for(int i = 0; i < n; i++)
    for(int j = i + 1; j < n; j++) {
      str_edge edge;
      edge.v1 = i;
      edge.v2 = j;
      edges.push_back(edge);

      // Getting edges weights ij and ji
      double weight = 0.0;
      for(int k = 0; k < c; k++)
        weight += getValue(x[k][i][j]);
      weights.push_back(weight);
      weight = 0.0;
      for(int k = 0; k < c; k++)
        weight += getValue(x[k][j][i]);
      weights.push_back(weight);
    }

  // if(logs)
	// 	_file << "Neighborhood list built!...\n";

  // Applying minimum cut algorithm
  vector<vector<int> > cuts;
  Gmindcut alg = Gmindcut(_file);
  alg.start(n, edges.size(), edges);
  int ncuts = alg.generate_cuts(weights, cuts, edges, false);
  _file << "Number of cuts found: " << ncuts << endl;

  _file << "Cuts matrix:\n";
  for(unsigned i = 0; i < cuts.size(); i++) {
    for(unsigned j = 0; j < cuts[i].size(); j++)
      _file << cuts[i][j] << "\t";
    _file << "\n";
  }

  // Evaluating whether there's constraint violation
  for(unsigned aux = 0; aux < cuts.size(); aux++) {
    vector<int> not_S;
    for(unsigned i = 0; i < n; i++)
      if(find(cuts[aux].begin(), cuts[aux].end(), i) == cuts[aux].end())
        not_S.push_back(i);

    IloExpr lhs(getEnv());
    for(unsigned i = 0; i < not_S.size(); i++)
      for(unsigned j = 0; j < cuts[aux].size(); j++)
        for(unsigned k = 0; k < c; k++)
          lhs += x[k][ not_S[i] ][ cuts[aux][j] ];

    IloConstraint cut = (lhs >= 1.0);
    cut = add(cut);
    if(logs)
      _file << cut << endl;
    cut.end();
    n_cuts++;
  }

  if(logs)
		_file << "Leaving hao_cutsetcallback\n";
}
