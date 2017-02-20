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

// void hao_cutsetcallback::main() {
//   bool logs = true;
// 	if(logs)
// 		_file << "\nEntering hao_cutsetcallback. Solution value:" << getObjValue() << "\n";
//
//   vector< double > weights;
//   for(unsigned it = 0; it < edges.size(); it++) {
//     unsigned i = edges[it].v1;
//     unsigned j = edges[it].v2;
//     weights.push_back(getValue(chi[it]));
//   }
//
//   if(logs)
// 		_file << "Neighborhood list built!...\n";
//
//   // Applying minimum cut algorithm
//   vector< vector< int > > cuts;
//   Gmindcut alg = Gmindcut(_file);
//   alg.start(trips.size() + 1, edges.size(), edges);
//   int ncuts = alg.generate_cuts(weights, cuts, edges, false);
//   _file << "Number of cuts found: " << ncuts << endl;
//
//   _file << "Cuts matrix:\n";
//   for(unsigned i = 0; i < cuts.size(); i++) {
//     for(unsigned j = 0; j < cuts[i].size(); j++)
//       _file << cuts[i][j] << "\t";
//     _file << "\n";
//   }
//
//   // Evaluating whether there's constraint violation
//   vector< int > not_S;
//   for(unsigned aux = 0; aux < cuts.size(); aux++) {
//     for(unsigned i = 0; i < trips.size() + 1; i++)
//       if(find(cuts[aux].begin(), cuts[aux].end(), i) == cuts[aux].end())
//         not_S.push_back(i);
//
//     IloExpr lhs(getEnv());
//     for(unsigned it = 0; it < edges.size(); it++) {
//       unsigned i = edges[it].v1;
//       if(find(not_S.begin(), not_S.end(), i) != not_S.end())
//         lhs += chi[it];
//     }
//
//     for(unsigned s = 0; s < cuts[aux].size(); s++) {
//       IloExpr rhs(getEnv());
//       for(unsigned k = 0; k < c; k++)
//         rhs += lambda[s][k];
//
//       IloConstraint cut = (lhs >= rhs);
//       cut = add(cut);
//       if(logs)
//         _file << cut << endl;
//       cut.end();
//       n_cuts++;
//     }
//   }
//
//   if(logs)
// 		_file << "Leaving hao_cutsetcallback\n";
// }
//
// void hao_cutsetcallback2::main() {
//   bool logs = true;
// 	if(logs)
// 		_file << "\nEntering hao_cutsetcallback. Solution value:" << getObjValue() << "\n";
//
//   vector< double > weights;
//   for(unsigned it = 0; it < edges.size(); it++) {
//     unsigned i = edges[it].v1;
//     unsigned j = edges[it].v2;
//     weights.push_back(getValue(chi[it]));
//   }
//
//   if(logs)
// 		_file << "Neighborhood list built!...\n";
//
//   // Applying minimum cut algorithm
//   vector< vector< int > > cuts;
//   Gmindcut alg = Gmindcut(_file);
//   alg.start(trips.size(), edges.size(), edges);
//   int ncuts = alg.generate_cuts(weights, cuts, edges, false);
//   _file << "Number of cuts found: " << ncuts << endl;
//
//   _file << "Cuts matrix:\n";
//   for(unsigned i = 0; i < cuts.size(); i++) {
//     for(unsigned j = 0; j < cuts[i].size(); j++)
//       _file << cuts[i][j] << "\t";
//     _file << "\n";
//   }
//
//   // Evaluating whether there's constraint violation
//   vector< int > not_S;
//   for(unsigned aux = 0; aux < cuts.size(); aux++) {
//     for(unsigned i = 0; i < trips.size() + 1; i++)
//       if(find(cuts[aux].begin(), cuts[aux].end(), i) == cuts[aux].end())
//         not_S.push_back(i);
//
//     IloExpr lhs(getEnv());
//     for(unsigned it = 0; it < edges.size(); it++) {
//       unsigned i = edges[it].v1;
//       if(find(not_S.begin(), not_S.end(), i) != not_S.end())
//         lhs += chi[it];
//     }
//
//     for(unsigned s = 0; s < cuts[aux].size(); s++) {
//       IloExpr rhs(getEnv());
//       for(unsigned k = 0; k < c; k++)
//         rhs += lambda[s][k];
//
//       IloConstraint cut = (lhs >= rhs);
//       cut = add(cut);
//       if(logs)
//         _file << cut << endl;
//       cut.end();
//       n_cuts++;
//     }
//   }
//
//   if(logs)
// 		_file << "Leaving hao_cutsetcallback\n";
// }
