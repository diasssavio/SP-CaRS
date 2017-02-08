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

void branch_callback::main() {
  IloInt const nbranch = getNbranches();
  node_info* data = dynamic_cast<node_info*>(getNodeData());
  if(nbranch > 0) { // If there's any branch to be made
    // Creating data in case of root branch-and-bound node, where data == NULL
    if(!data) {
      if(!node_info::root_data)
        node_info::init_root_data();
      data = node_info::root_data;
      root_lb = getObjValue();
      if(this->hasIncumbent())
        root_gap = this->getMIPRelativeGap();
      else
        root_gap = -1;
      root_time = ((double) (timer.getMilliSpan() / 1000));
      branched = true;
    }

    // Execution ends at root node and a new model is solved.
    unsigned depth = data->get_depth();
    if(depth > 0) {
      int c = cars.get_c();
      int n = cars.get_n();
      IloEnv env = getEnv();

      env.out() << "---- STARTING MINIMUM CUT EXECUTION ----\n";
      // IloNumArray3 _x(env);
      // IloNumArray3 _y(env);
      for(int k = 0; k < c; k++) {
        env.out() << "---- VEHICLE " << k << " ----\n";
        IloNumArray2 aux1(env);
        IloNumArray2 aux2(env);
        for(int i = 0; i < n; i++) {
          IloNumArray aux3(env);
          IloNumArray aux4(env);
          for(int j = 0; j < n; j++) {
            aux3.add(getValue(x[k][i][j]));
            aux4.add(getValue(y[k][i][j]));
          }
          aux1.add(aux3);
          aux2.add(aux4);
        }
        // _x.add(aux1);
        // _y.add(aux2);

        mincut_model mod(env, cars, aux1, aux2);
        IloCplex cplex(mod);
    		cplex.setParam(IloCplex::Threads, 1);
    		cplex.setParam(IloCplex::Param::Preprocessing::Aggregator, 0);
    		cplex.setParam(IloCplex::Param::Preprocessing::Presolve, 0);
  			cplex.setParam(IloCplex::PreInd, 0);
  			cplex.setParam(IloCplex::CutsFactor, 1);
  			cplex.setParam(IloCplex::HeurFreq, -1);
        cplex.solve();

        env.out() << "OBJECTIVE VALUE: " << fixed << cplex.getObjValue() << endl;
      }

      env.out() << "---- FINISHING MINIMUM CUT EXECUTION ----\n";
      abort();
      return;
    }

    // Proceding with CPLEX standard branching
    IloNumVarArray vars(getEnv());
    IloNumArray bounds(getEnv());
    IloCplex::BranchDirectionArray dirs(getEnv());
    for(IloInt i = 0; i < nbranch; i++) {
      IloNum const est = getBranch(vars, bounds, dirs, i);
      makeBranch(vars, bounds, dirs, est, new node_info(depth + 1U));
    }

    dirs.end();
    bounds.end();
    vars.end();
  } else { // Else, prune the node
    prune();
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
