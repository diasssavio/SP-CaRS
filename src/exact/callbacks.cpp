//============================================================================
// Name        : callbacks.cpp
// Author      : Sávio S. Dias
// E-mail      : diasssavio@gmail.com
// Institution : Universidade Federal Fluminense
// Description : Callbacks class implementation
//============================================================================

#include "../include/callbacks.h"

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

void subset_callback::main() {
  IloEnv env = getEnv();
	IloInt c = cars.get_c();
	IloInt n = cars.get_n();

	bool logs = true;
	if(logs)
		_file << "Entering callback Subset1...\n";

	vector< bool > visited(n, false);
	IloInt i = 0;
	while(i < n) {
		if(visited[i]) {
			i++;
			continue;
		}

		// Checking whether there's a j visited by any vehicle k right after i
		IloInt k, j;
		bool visited_by_k = false;
		for(j = 0; j < n; j++) {
			if(j == i) continue;
			for(k = 0; k < c; k++)
				if(getValue(x[k][i][j]) > 0.5) {
					visited_by_k = true;
					break;
				}
			if(visited_by_k) break;
		}

		// _file << k << ": ";
		// _file << "Vertex " << i << " visited by " << k << endl;
		visited[i] = true;
		IloInt vertices = 1;

		IloExpr expr(env);
		IloExpr expr1(env);

		visited[j] = true;
		vertices++;

		// Adding the current variable to both constraints
		expr += x[k][i][j];
		expr1 += x[k][j][i];

		if(logs)
			_file << "x(" << k << ")(" << i << ")(" << j << ") + ";

		// While there isn't an edge returning to i using any vehicle
		while(getValue(x[k][j][i]) <= 0.5) {
			IloInt aux = j;
			bool flag = false;
			for(j = 0; j < n; j++) {
				if(j == i) continue;
				if((getValue(x[k][aux][j]) > 0.5) && !visited[j]) {
					flag = true;
					break;
				}
			}

			if(!flag) {
				j = aux;
				k = (k + 1) % c;
				continue;
			}

			visited[j] = true;
			vertices++;

			expr += x[k][aux][j];
			expr1 += x[k][j][aux];

			if(logs)
				_file << "x(" << k << ")(" << aux << ")(" << j << ") + ";
		}

		expr += x[k][j][i];
		expr1 += x[k][i][j];

		if(logs)
			_file << "x(" << k << ")(" << j << ")(" << i << ")";

		IloConstraint cut = (expr <= vertices - 1);
		IloConstraint cut1 = (expr1 <= vertices - 1);

		// _file << "Adding cuts\n";
		if(vertices < n) {
			if(logs)
				_file << " <= " << vertices - 1 << endl;
			add(cut).end();
			add(cut1).end();
		}

		i++;
	}
	if(logs)
		_file << "Leaving callback Subset1\n";
}

void subset_callback2::main() {
  IloEnv env = getEnv();
	IloInt c = cars.get_c();
	IloInt n = cars.get_n();

  bool logs = true;
	if(logs)
		_file << "Entering callback Subset2...\n";

  vector< bool > visited(n, false);
	IloInt i = 0, k = 0;
  while(i < n) {
		if(visited[i]) {
			i++;
			continue;
		}

    // Checking whether there's a j visited by vehicle k right after i
		IloInt j;
		bool visited_by_k = false;
		for(j = 0; j < n; j++) {
			if(j == i) continue;
			if(getValue(x[k][i][j]) > 0.5) {
				visited_by_k = true;
				break;
			}
		}

    visited[i] = true;
		IloInt vertices = 1;

		IloExpr expr(env);
		IloExpr expr1(env);

		visited[j] = true;
		vertices++;

		// Adding the current variable to both constraints
		expr += x[k][i][j];
		expr1 += x[k][j][i];

    if(logs)
			_file << "x(" << k << ")(" << i << ")(" << j << ") + ";

    // While there isn't an edge returning to i using vehicle k
		while(getValue(x[k][j][i]) <= 0.5) {
			IloInt aux = j;
			bool flag = false;
			for(j = 0; j < n; j++) {
				if(j == i) continue;
				if((getValue(x[k][aux][j]) > 0.5) && !visited[j]) {
					flag = true;
					break;
				}
			}

			if(!flag) {
				k++;
				break;
			}

			visited[j] = true;
			vertices++;

			expr += x[k][aux][j];
			expr1 += x[k][j][aux];

			if(logs)
				_file << "x(" << k << ")(" << aux << ")(" << j << ") + ";
		}

    expr += x[k][j][i];
		expr1 += x[k][i][j];

		if(logs)
			_file << "x(" << k << ")(" << j << ")(" << i << ")";

		IloConstraint cut = (expr <= vertices - 1);
		IloConstraint cut1 = (expr1 <= vertices - 1);

		// _file << "Adding cuts\n";
		if(vertices < n) {
			if(logs)
				_file << " <= " << vertices - 1 << endl;
			add(cut).end();
			add(cut1).end();
		}

		i++;
  }
}

void cutset_callback::main() {
  unsigned c = cars.get_c();
  unsigned n = cars.get_n();

  bool logs = true;
	if(logs)
		_file << "\nEntering cutset_callback. Solution value:" << getObjValue() << "\n";

  // Building Neighborhood list and weights
  vector<set<unsigned> > neighbors;
  vector<vector<double> > weights;
  for(unsigned i = 0; i < n; i++) {
    set<unsigned> aux;
    vector<double> line;
    for(unsigned j = 0; j < n; j++) {
      if(i == j) {
        line.push_back(0.0);
        continue;
      }

      double temp = 0.0;
      for(unsigned k = 0; k < c; k++) {
        if(getValue(x[k][i][j]) > EPSILON)
          aux.insert(j);
        temp += getValue(x[k][i][j]);
        // _file << getValue(x[k][i][j]) << " ";
      }
      line.push_back(temp);
      // _file << "\n";
    }
    neighbors.push_back(aux);
    weights.push_back(line);
  }

  // if(logs)
	// 	_file << "Neighborhood list built!...\n";

  // Applying minimum cut algorithm
  mincut alg = mincut();
  alg.run(n, neighbors, weights);
  // if(logs)
  //   alg.print_graph(_file);

  // Evaluating whether there's constraint violation
  double cut_cost = alg.generate_min_cut();
  if(logs)
    _file << "Cut cost:" << cut_cost << ".\t";
  if(cut_cost < 1.0 - EPSILON) {
    if(logs)
  		_file << "Min Cut violated!\n";

    IloExpr lhs(getEnv());
    for(unsigned i = 0; i < n; i++)
      if(!alg.is_node_in_cut(i))
        for(unsigned j = 0; j < n; j++) {
          if(i == j) continue;
          if(alg.is_node_in_cut(j))
            for(unsigned k = 0; k < c; k++)
              lhs += x[k][i][j];
        }

    IloConstraint cut = (lhs >= 1.0);
    cut = add(cut);
    if(logs)
      _file << cut;
    cut.end();
    n_cuts++;

    // if(logs) {
    //   for(int k = 0; k < c; k++)
    //     for(int i = 0; i < n; i++)
    //       for(int j = 0; j < n; j++)
    //         if(getValue(x[k][i][j]) > EPSILON)
    //           _file << k << " " << i << " " << j << ": " << getValue(x[k][i][j]) << "\n";
    // }
  }

  if(logs)
		_file << "\nLeaving cutset_callback\n";
}

void cutset_callback2::main() {
  unsigned c = cars.get_c();
  unsigned n = cars.get_n();

  bool logs = true;
	if(logs)
		_file << "\nEntering cutset_callback2. Solution value:" << getObjValue() << "\n";

  // Building Neighborhood list and weights
  vector<set<unsigned> > neighbors;
  vector<vector<double> > weights;
  for(unsigned i = 0; i < n; i++) {
    set<unsigned> aux;
    vector<double> line;
    for(unsigned j = 0; j < n; j++) {
      if(i == j) {
        line.push_back(0.0);
        continue;
      }

      double temp = 0.0;
      for(unsigned k = 0; k < c; k++) {
        if(getValue(x[k][i][j]) > EPSILON)
          aux.insert(j);
        temp += getValue(x[k][i][j]);
        // _file << getValue(x[k][i][j]) << " ";
      }
      line.push_back(temp);
      // _file << "\n";
    }
    neighbors.push_back(aux);
    weights.push_back(line);
  }

  // if(logs)
	// 	_file << "Neighborhood list built!...\n";

  // Applying minimum cut algorithm
  mincut alg = mincut();
  alg.run(n, neighbors, weights);
  // if(logs)
  //   alg.print_graph(_file);

  // Evaluating whether there's constraint violation
  double cut_cost = alg.generate_min_cut();
  if(logs)
    _file << "Cut cost:" << cut_cost << ".\t";
  if(cut_cost < 1.0 - EPSILON) {
    if(logs)
  		_file << "Min Cut violated!\n";

    IloExpr lhs(getEnv());
    for(unsigned i = 0; i < n; i++)
      if(!alg.is_node_in_cut(i))
        for(unsigned j = 0; j < n; j++) {
          if(i == j) continue;
          if(alg.is_node_in_cut(j))
            for(unsigned k = 0; k < c; k++)
              lhs += x[k][i][j];
        }

    IloConstraint cut = (lhs >= 1.0);
    cut = add(cut);
    if(logs)
      _file << cut;
    cut.end();
    n_cuts++;
  }

  if(logs)
		_file << "\nLeaving cutset_callback2\n";
}

void cutset_callback3::main() {
  unsigned c = cars.get_c();
  unsigned n = cars.get_n();

  bool logs = true;
	if(logs)
		_file << "\nEntering cutset_callback3. Solution value:" << getObjValue() << "\n";

  // Building Neighborhood list and weights
  vector<set<unsigned> > neighbors;
  vector<vector<double> > weights;
  for(unsigned i = 0; i < n; i++) {
    set<unsigned> aux;
    vector<double> line;
    for(unsigned j = 0; j < n; j++) {
      if(i == j) {
        line.push_back(0.0);
        continue;
      }

      double temp = 0.0;
      for(unsigned k = 0; k < c; k++) {
        if(getValue(x[k][i][j]) > EPSILON)
          aux.insert(j);
        temp += getValue(x[k][i][j]);
        // _file << getValue(x[k][i][j]) << " ";
      }
      line.push_back(temp);
      // _file << "\n";
    }
    neighbors.push_back(aux);
    weights.push_back(line);
  }

  // if(logs)
	// 	_file << "Neighborhood list built!...\n";

  // Applying minimum cut algorithm
  mincut alg = mincut();
  alg.run(n, neighbors, weights);
  // if(logs)
  //   alg.print_graph(_file);

  // Evaluating whether there's constraint violation
  double cut_cost = alg.generate_min_cut();
  if(logs)
    _file << "Cut cost:" << cut_cost << ".\t";
  if(cut_cost < 1.0 - EPSILON) {
    _file << "x:\n";
    for(unsigned i = 0; i < n; i++)
      for(unsigned j = 0; j < n; j++)
        for(unsigned k = 0; k < c; k++)
          if(getValue(x[k][i][j]) > EPSILON)
            _file << k << " " << i << " " << j << ": " << getValue(x[k][i][j]) << "\n";
    _file << "\ny:\n";
    for(unsigned i = 0; i < n; i++)
      for(unsigned j = 0; j < n; j++)
        for(unsigned k = 0; k < c; k++)
          if(getValue(y[k][i][j]) > EPSILON)
            _file << k << " " << i << " " << j << ": " << getValue(y[k][i][j]) << "\n";

    IloExpr lhs(getEnv());
    for(unsigned i = 0; i < n; i++)
      if(!alg.is_node_in_cut(i))
        for(unsigned j = 0; j < n; j++) {
          if(i == j) continue;
          if(alg.is_node_in_cut(j))
            for(unsigned k = 0; k < c; k++)
              lhs += x[k][i][j];
        }

    IloConstraint cut = (lhs >= 1.0);
    cut = add(cut);
    if(logs)
      _file << cut;
    cut.end();
    n_cuts++;

    if(logs)
  		_file << "\nLeaving cutset_callback3\n";
    // exit(0);
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
