//============================================================================
// Name        : gmindcut.cpp
// Author      : Sávio S. Dias
// E-mail      : diasssavio@gmail.com
// Institution : Universidade Federal Fluminense
// Description : Gmindcut class implementation
//============================================================================

#include "../include/gmindcut.h"

// double Gmindcut::tempo = 0;
int Gmindcut::nx = 0;
int Gmindcut::ncut = 0;

Gmindcut::Gmindcut(ofstream& file) : _file(file) {
  gr.n_nodes = 0;
  gr.n_arcs = 0;
  gr.n_terminals = 0;
  gr.Wptr = NULL;
  gr.raiz = -1;
  gr.sink = -1;
}

Gmindcut::~Gmindcut() { }

void Gmindcut::start(const int n_vert, const int n_arest, const vector<str_edge>& ar) {
  int n = n_vert;
  int m = n_arest;

  alloc_graph(n, m);
  gr.n_nodes = n;
  gr.n_arcs = m;
  gr.n_terminals = n_vert;
  gr.raiz = 1;
  gr.sink = -1;

  number = vector<int>(n);

  node *nptr;
  int i;
  for(i = n, nptr = &(gr.nodes[n-1]); i > 0; nptr--, i--) {
    nptr->id = i;
    nptr->first_arc = NULL;
    nptr->terminal = true;
  }

  arc* aptr1 = &(gr.arcs[0]);
  arc* aptr2 = &(gr.arcs[m]);
  for(i = 0; i < n_arest; i++, aptr1++, aptr2++) {
    int nod1 = ar[i].v1;
    int nod2 = ar[i].v2;

    node* nptr1 = &(gr.nodes[nod1]);
    node* nptr2 = &(gr.nodes[nod2]);
    aptr1->adjac = nptr2;
    aptr2->adjac = nptr1;
    aptr1->back = aptr2;
    aptr2->back = aptr1;

    if(nptr1->first_arc == NULL) {
      nptr1->first_arc = aptr1;
      aptr1->next = aptr1;
    } else {
      aptr1->next = nptr1->first_arc->next;
      nptr1->first_arc->next = aptr1;
    }

    if(nptr2->first_arc == NULL) {
      nptr2->first_arc = aptr2;
      aptr2->next = aptr2;
    } else {
      aptr2->next = nptr2->first_arc->next;
      nptr2->first_arc->next = aptr2;
    }

    aptr1->index = 2 * i; // x_ij index
    aptr2->index = 2 * i + 1; // x_ji index
  }
}

void Gmindcut::alloc_graph(const int n, const int m) {
  gr.nodes = vector<node>(n);
  gr.arcs = vector<arc>(2 * m);
}

void Gmindcut::de_alloc_graph() {
  gr.nodes.clear();
  gr.arcs.clear();
}

void Gmindcut::initialize() {
  int n = gr.n_nodes;
  int r = gr.raiz;
  int id;
  node* nptr;
  for(nptr = &(gr.nodes[0]), id = 1; id <= n; nptr++, id++)
    if(nptr->unmarked) {
      nptr->in_S = false;
      nptr->in_W = false;
    } else if(id == r) {
      nptr->in_S = true;
      nptr->in_W = false;
    } else {
      nptr->in_S = false;
      nptr->in_W = true;
    }

  node* first = NULL;
  node* last = NULL;
  for(nptr = &(gr.nodes[0]), id = 1; id <= n; nptr++, id++) {
    if(!nptr->in_W) continue;
    nptr->scan_ptr = nptr->first_arc;
    nptr->excess = 0.0;
    nptr->stack_link = NULL;
    if(!first) {
      first = last = nptr;
      first->right_link = nptr;
      first->left_link = nptr;
    } else {
      last->right_link = nptr;
      nptr->left_link = last;
      nptr->right_link = first;
      first->left_link = nptr;
      last = nptr;
    }
  }

  gr.Wptr = last;
}

int Gmindcut::bfs0(node* t) {
  int level, count;
  node *q_rear, *q_front, *nptr;
  arc *aptr;

  t->dist = 0;
  count = 1;
  t->unmarked = false;
  q_front = t;
  q_rear = q_front;

bfs_next:
  level = q_rear->dist + 1;
  aptr = q_rear->first_arc;
  do {
    if (aptr->adjac->unmarked && aptr->back->rcap > EPS) {
		  nptr = aptr->adjac;
		  nptr->unmarked = false;
		  nptr->dist = level;
		  ++number[level];
		  if(nptr->terminal) ++count;
		  q_front->bfs_link = nptr;
		  q_front = nptr;
		}
    aptr = aptr->next;
  } while (aptr != q_rear->first_arc);

	if (q_rear == q_front) goto bfs_ready;
	q_rear = q_rear->bfs_link;
	goto bfs_next;

bfs_ready: ;
	return count;
}

void Gmindcut::bfs1 (node *t) {
  node *q_front, *q_rear, *nptr;
  arc *aptr;
  int level;

  t->unmarked = false;
  t->dist = 0;
  q_front = t;
  q_rear = q_front;

bfs_next:
  level = q_rear->dist + 1;
  aptr = q_rear->first_arc;
  do {
    if (aptr->adjac->in_W && aptr->adjac->unmarked && aptr->back->rcap > EPS) {
		  nptr = aptr->adjac;
		  nptr->unmarked = false;
		  nptr->dist = level;
		  q_front->bfs_link = nptr;
		  q_front = nptr;
		}
    aptr = aptr->next;
  } while (aptr != q_rear->first_arc);
  if (q_rear == q_front)
    goto bfs_ready;

  q_rear = q_rear->bfs_link;
  goto bfs_next;

bfs_ready: ;
}

bool Gmindcut::gmincut(double& mincap, int& n_shore, int& ncuts, const bool global, vector<str_cut*>& cuts) {
  /* Determines global minimum cut in a directed graph, i.e.
     a cut of minimum capacity with respect to cuts between
     all pairs of nodes.

     References:
     ----------
     J. Hao/ J.B. Orlin: "A Faster Algorithm for Finding
     the Minimum Cut in a Graph", Proc. of the 3rd Annual
     ACM-SIAM Symposium on Discrete Algorithms, Orlando,
     Florida, 1992
  */

  node *s_ptr, *t_ptr, *W_ptr, *w_end_ptr, *actv_ptr, *nptr, *nnptr, *W1_ptr, *D_ptr, *D1_ptr;
  node **dor_ptr, **ptr;
  int n, s, t, card_S, card_TS, max_dor, max_dist, id;
  arc *aptr;
  int dmin = 9999999;
  double incre, cap;
  int adist, i, k;
  bool found,achei;

  mincap = MAX_DOUBLE;
  n_shore = -1;
  n = gr.n_nodes;
  s = gr.raiz;

  vector<node*> dormant = vector<node*>(n);
  vector<node*> active = vector<node*>(n + 1, NULL);
  s_ptr = &(gr.nodes[s-1]);

  // Chosing a terminal as drain
  if(gr.sink != -1 && gr.sink != s && gr.nodes[ gr.sink - 1 ].terminal) {
    t_ptr = &(gr.nodes[ gr.sink - 1 ]);
    t = gr.sink;
  } else {
    for(int i = n - 1, t = -1; i >= 0; i--)
      if(gr.nodes[i].terminal && (i != s - 1)) {
        t_ptr = &(gr.nodes[i]);
        t = t_ptr->id;
        break;
      }
  }

  if(!t_ptr) {
    _file << "Gmindcut::gmincut - There must be at least two terminals\n";
    return false;
  }

  // Breadth first search to determine if each terminal is reached from the root
  for(nptr = &(gr.nodes[0]), id = 1; id <= n; nptr++, id++) nptr->unmarked = true;
  k = bfs0(s_ptr);
  if(k < gr.n_terminals) {
    _file << "Gmindcut::gmincut - Terminals not reachable from the root\n";
    return false;
  }

  initialize();

  s_ptr->in_S = true;
  s_ptr->in_W = false;
  card_S = 1;
  card_TS = 1;

  // Breadth first search to get exact distances from first sink
  for(nptr = &(gr.nodes[0]), id = 1; id <= n; nptr++, id++) nptr->unmarked = true;
  bfs0(t_ptr);

  number[s - 1] = 1;
  W_ptr = gr.Wptr;

  // Initializing set of dormant nodes
  dormant[0] = s_ptr;
  max_dor = 0;
  s_ptr->left_link = s_ptr;
  s_ptr->right_link = s_ptr;

  // Initial preflow push from node s
  max_dist = 0;
  aptr = s_ptr->first_arc;
  do {
    nptr = aptr->adjac;
    if (aptr->rcap > EPS) {
  		cap = aptr->rcap;
      nptr->excess += cap;
      s_ptr->excess -= cap;
      aptr->back->rcap += cap;
      aptr->rcap = 0.0;

      if (nptr != t_ptr && nptr->excess <= cap + EPS) {
      	// Push node nptr onto stack for nptr->dist, but only once in case of double arcs
	      nptr->stack_link = active[nptr->dist];
	      active[nptr->dist] = nptr;
	      if (nptr->dist > max_dist)
					max_dist = nptr->dist;
	    }
    }
    aptr = aptr->next;
  } while(aptr != s_ptr->first_arc);

  // MAIN LOOP
next_cut:
    do { /* get maximum distance active node */
    actv_ptr = active[max_dist];
    while (actv_ptr != NULL) { /* remove node *actv_ptr from stack */
      active[max_dist] = actv_ptr->stack_link;
      aptr = actv_ptr->scan_ptr;

           /* node *actv_ptr will not be put back onto stack
            again in current mincut computation, either it
            is processed until its excess becomes zero or
            else it will go into the set of dormant nodes */
arc_scan:   /* for current active node */
	  	nptr = aptr->adjac;
	  	if (nptr->in_W && nptr->dist == actv_ptr->dist-1 && aptr->rcap > EPS) {
	      incre = actv_ptr->excess;
	      if (incre <= aptr->rcap) { /* perform a non saturating push */
				  aptr->rcap -= incre;
				  aptr->back->rcap += incre;
				  actv_ptr->excess = 0.0;
				  nptr->excess += incre;
	  			if (nptr != t_ptr && nptr->excess <= incre + EPS) {
			      nptr->stack_link = active[nptr->dist];
			      active[nptr->dist] = nptr;
			    }
				  actv_ptr->scan_ptr = aptr;
				  goto node_ready;
				} else { /* perform a saturating push */
				  incre = aptr->rcap;
				  aptr->back->rcap += incre;
				  actv_ptr->excess -= incre;
				  nptr->excess += incre;
				  aptr->rcap = 0.0;
				  if (nptr != t_ptr && nptr->excess <= incre + EPS) {
			      nptr->stack_link = active[nptr->dist];
			      active[nptr->dist] = nptr;
			    }
				  if (actv_ptr->excess <= EPS) {
			      actv_ptr->scan_ptr = aptr;
			      goto node_ready;
			    }
				}
	    }
  		if (aptr->next == actv_ptr->first_arc) {
		  /* all admissable arcs of current active node
			 	 scanned, relabel or update set of dormant
		     nodes now */
		    adist = actv_ptr->dist;
		    if (number[adist] == 1) {
			  	/* dist[j] != dist[i] for all j in W-{i},
			       extend dormant set by another layer */
				  dor_ptr = &(dormant[++max_dor]);
				  *dor_ptr = NULL;
				  nptr = W_ptr;
				  w_end_ptr = W_ptr->left_link;

transfer:
			  	nnptr = nptr->right_link;
			  	if (nptr->dist >= adist) {
			      /* remove node nptr from set W */
			      nptr->left_link->right_link = nptr->right_link;
			      nnptr->left_link = nptr->left_link;
			      if (W_ptr == nptr)
							W_ptr = nnptr;
			      /* W_ptr != NULL since t_ptr
				 			 is  contained in W       */
			      nptr->in_W = false;
			      --number[nptr->dist];

			      /* clear stack for nptr->dist */
			      active[nptr->dist] = NULL;
			      nptr->scan_ptr = nptr->first_arc;

			      /* put node nptr into linked list
				 dormant[max_dor]              */
			      if (*dor_ptr == NULL) {
						  *dor_ptr = nptr;
						  nptr->right_link = nptr;
						  nptr->left_link = nptr;
						} else {
						  nptr->right_link = *dor_ptr;
						  nptr->left_link = (*dor_ptr)->left_link;
						  (*dor_ptr)->left_link = nptr;
						  nptr->left_link->right_link = nptr;
						}
			    }
			  	if (nptr == w_end_ptr)
			    	goto node_ready;

				  nptr = nnptr;
				  goto transfer;

				} else {
			  	/* check if there is an arc (u, v), u=*actv_ptr,
			       such that v in W and rcap(u,v) > 0  */
				  aptr = actv_ptr->first_arc;
				  found = false;
				  do {
				    if (aptr->adjac->in_W && aptr->rcap > EPS) {
						  found = true;
						  dmin = aptr->adjac->dist;
						  break;
						} else
				      aptr = aptr->next;
		      } while (aptr != actv_ptr->first_arc);
				  if (found) {
			      actv_ptr->scan_ptr = aptr;

			      /* get new distance label for *actv_ptr */
				    while (aptr->next != actv_ptr->first_arc) {
						  aptr = aptr->next;
						  if (aptr->adjac->in_W && aptr->adjac->dist < dmin && aptr->rcap > EPS)
						    dmin = aptr->adjac->dist;
						}
			      --number[adist];
			      actv_ptr->dist = dmin + 1;
			      ++number[dmin+1];
			      max_dist = dmin + 1;
			      aptr = actv_ptr->scan_ptr;
			      goto arc_scan;
				  } else {
				  	/* extend dormant set by another
				       layer containing node *actv_ptr only,
				       remove actv_ptr from W nodes first    */

			      actv_ptr->in_W = false;
			      --number[adist];
			      actv_ptr->scan_ptr = actv_ptr->first_arc;
			      actv_ptr->left_link->right_link = actv_ptr->right_link;
			      actv_ptr->right_link->left_link = actv_ptr->left_link;
			      if (W_ptr == actv_ptr)
							W_ptr = actv_ptr->right_link;
			      dormant[++max_dor] = actv_ptr;
			      actv_ptr->right_link = actv_ptr;
			      actv_ptr->left_link = actv_ptr;
			      goto node_ready;
				  }
				}
		  } else {
		    aptr = aptr->next;
		    goto arc_scan;
		  }

node_ready:
			actv_ptr = active[max_dist];
		} // actv_ptr != NULL
	  --max_dist;
	} while (max_dist > 0);

check_min:
  if (t_ptr->excess < mincap ) {
    mincap = t_ptr->excess;
    gr.sink = t_ptr->id;    // Sink for future iterations
    n_shore = 0;
    for (nptr = &(gr.nodes[n-1]); nptr >= &(gr.nodes[0]); --nptr) {
		  if (nptr->in_W)
		    nptr->shore1 = false;
		  else {
	      nptr->shore1 = true;
	      ++n_shore;
	    }
		}
    if( !global ) goto ready;
  }

  if(t_ptr->excess < 1.0 - 2 * EPSILON) {
    if(verif_corte(t_ptr->excess, t_ptr->id-1, true))
		add_cut(ncuts,cuts, t_ptr->id-1, 1 - t_ptr->excess);
  }

  if (t_ptr->excess < EPS) {
  	// Current sink did not receive any flow, i.e. there is no directed path to it from the source
    mincap = 0.0;
    goto ready;
  }

  ++card_TS;
  if (card_TS == gr.n_terminals)
    goto ready;

  /* delete t_ptr from W */
  t_ptr->in_W = false;
  --number[t_ptr->dist];
  if (t_ptr->right_link == t_ptr)
    W_ptr = NULL;
  else {
	  t_ptr->right_link->left_link = t_ptr->left_link;
	  t_ptr->left_link->right_link = t_ptr->right_link;
	  if (W_ptr == t_ptr)
	    W_ptr = t_ptr->right_link;
	}

  /* put t_ptr into source set S and dormant[0] set */
  t_ptr->in_S = true;
  ++card_S;
  t_ptr->right_link = dormant[0]->right_link;
  t_ptr->left_link = dormant[0];
  dormant[0]->right_link->left_link = t_ptr;
  dormant[0]->right_link = t_ptr;

  /* saturate all arcs from *t_ptr to nodes not in S */
  aptr = t_ptr->first_arc;
  do {
    nptr = aptr->adjac;
    if (! nptr->in_S  && aptr->rcap > EPS) {
		  t_ptr->excess -= aptr->rcap;
		  nptr->excess += aptr->rcap;
		  aptr->back->rcap += aptr->rcap;
		  aptr->rcap = 0.0;
		  nptr->scan_ptr = nptr->first_arc;
		}
    aptr = aptr->next;
  } while (aptr != t_ptr->first_arc);

  if (W_ptr == NULL) {
  	/* set of W nodes empty, dormant[max_dor]
       taken as next set of W nodes */
    W_ptr = dormant[max_dor--];
    nptr = W_ptr;
    do {
		  nptr->in_W = true;
		  nptr = nptr->right_link;
		} while (nptr != W_ptr);
  }

  // Enquanto W nao tiver um terminal, vai adicionando conjuntos dormentes a W
  achei = false;
  nptr = W_ptr;
  do {
    if (nptr->terminal) {
		  achei = true;
		  break;
		}
    nptr = nptr->right_link;
  } while (nptr != W_ptr);

  while( !achei ) {
    if( max_dor < 1 ) {
		  //std::cout<<"Gmindcut::gmincut - WARNING:conjunto dormente 0 adicionado a W"<<std::endl;
		  //printf("WARNING:conjunto dormente 0 adicionado a W\n");
		  goto ready;
		}
    nptr = D_ptr = dormant[max_dor--];
    do {
		  nptr->in_W = true;
		  if (nptr->terminal) achei = true;
		  nptr = nptr->right_link;
		} while (nptr != D_ptr);
    W1_ptr = W_ptr->right_link;
    D1_ptr = D_ptr->right_link;
    W_ptr->right_link = D1_ptr;
    D1_ptr->left_link = W_ptr;
    W1_ptr->left_link = D_ptr;
    D_ptr->right_link = W1_ptr;
  }

  /* get TERMINAL node from W with minimum distance as new sink */
  dmin = 9999999;
  t_ptr = W_ptr;
  nptr = t_ptr;
  while (nptr->right_link != W_ptr) {
    nptr = nptr->right_link;
    //printf("%d ",nptr->id);
    if ( nptr->terminal && nptr->dist < dmin) {
		  //printf("+");
		  dmin = nptr->dist;
		  t_ptr = nptr;
		}
  }

  /* breadth first search to get exact distances
     for nodes of W with respect to new sink,
     nodes of W with positive excess will be pushed
     onto stack of active nodes, not all nodes of W
     are reachable by breadth first search in the
     residual graph, however, all such nodes are put
     into another dormant set                       */
  if (W_ptr->right_link == W_ptr)
    /* only one node left in W = new sink */
    goto check_min;

  for (i = n-1, ptr = &(active[n-1]); i >= 0; i--, ptr--) {
    number[i] = 0;
    *ptr = NULL;
  }
  nptr = t_ptr;
  while (nptr->right_link != t_ptr) {
    nptr = nptr->right_link;
    nptr->unmarked = true;
    nptr->scan_ptr = nptr->first_arc;
  }

  max_dist = 0;
  number[0] = 1;

  bfs1 (t_ptr);

  /*   check next set W for nodes to be transferred
       to another dormant set and for active nodes
       to be pushed onto stack                      */
  dor_ptr = &(dormant[max_dor+1]);
  *dor_ptr = NULL;
  nptr = W_ptr;
  w_end_ptr = W_ptr->left_link;

check_W:
  nnptr = nptr->right_link;
  if (nptr->unmarked) {
    /* remove node *nptr from set W */
    nptr->in_W = false;
    nptr->right_link->left_link = nptr->left_link;
    nptr->left_link->right_link = nptr->right_link;
    if (W_ptr == nptr)
      W_ptr = nnptr;

    /* put node *nptr into new set dormant[dor_max+1] */
    if (*dor_ptr == NULL) {
		  *dor_ptr = nptr;
		  nptr->right_link = nptr;
		  nptr->left_link = nptr;
		} else {
		  nptr->right_link = (*dor_ptr)->right_link;
		  nptr->left_link = (*dor_ptr);
		  nptr->right_link->left_link = nptr;
		  (*dor_ptr)->right_link = nptr;
		}
  } else if (nptr != t_ptr) {
		++number[nptr->dist];
    if (nptr->excess > EPS) {
	    nptr->stack_link = active[nptr->dist];
	    active[nptr->dist] = nptr;
	    if (nptr->dist > max_dist)
	      max_dist = nptr->dist;
	  }
  }
  if (nptr == w_end_ptr)
    goto end_check;

  nptr = nnptr;
  goto check_W;

end_check:
  if (*dor_ptr != NULL)
    ++max_dor;

  goto next_cut;

ready: ;

  return true;
}

void Gmindcut::write_graph() const {
  int m = gr.n_arcs, j;

  _file <<"Gmindcut::write_graph - n:"<<gr.n_nodes<<" m:"<<m<<" root:"<<gr.raiz-1<<" Terminals:"<<gr.n_terminals<<std::endl;
  for (j = 0; j < gr.n_nodes; ++j)
    _file <<"id: "<<gr.nodes[j].id-1<<" Terminal: "<<gr.nodes[j].terminal/*<<" V.Real: "<<gr.nodes[j].v_real*/<<std::endl;

  for (j = 0; j < m; ++j) {
	  _file <<"("<<gr.arcs[j].back->adjac->id-1<<","<<gr.arcs[j].adjac->id-1;
	  _file <<") rcap:"<<gr.arcs[j].rcap<<" rcap:"<<gr.arcs[j].back->rcap;
	  _file <<" cap:"<<gr.arcs[j].cap<<" cap:"<<gr.arcs[j].back->cap;//<<std::endl;
	  _file <<" index: "<<gr.arcs[j].index<<" index: "<<gr.arcs[j].back->index<<std::endl;
  }
  _file <<"Gmindcut::write_graph - fim"<<std::endl;
}

int Gmindcut::generate_cuts(const vector<double>& x, vector<vector<int> >& sol, const vector<str_edge>& arc, const bool only_best) {
  // unsigned long int ini = clock();
  int i, j, ncuts, n_shore, n_vert = gr.n_nodes - 1;
  double cap;
  bool err;

  vector<str_cut*> cuts(gr.n_terminals, NULL);
  ++nx;
  for(i = gr.n_arcs, j = 0; j < arc.size(); ++i, ++j) {
    gr.arcs[i].rcap = x[gr.arcs[i].index] + EPSILON; // residual capacity
    gr.arcs[i].cap = x[gr.arcs[i].index]; // capacity

    gr.arcs[j].rcap = x[gr.arcs[j].index] + EPSILON;
    gr.arcs[j].cap = x[gr.arcs[j].index];
  }

  ncuts = 0;
  err = gmincut (cap, n_shore, ncuts, true, cuts);
  if(!err)
    _file << "Gmindcut::generate_cuts - cortes não gerados\n";

  if(ncuts > 0) generate_cuts(sol, ncuts, cuts, only_best);
  // tempo += calcula_tempo(ini, clock());

  return ncuts;
}

void Gmindcut::add_cut(int& ncuts, vector<str_cut*>& cuts, const int v, const double viol) const {
  int tam, i, n_vert = gr.n_nodes - 1;
  arc *aptr1;
  double val;

  cuts[ncuts] = new str_cut;
  cuts[ncuts]->S = vector<int>(gr.n_nodes, -1);
  // cuts[ncuts]->S.redefine(gr.n_nodes, -1);
  cuts[ncuts]->viol = 0;
  cuts[ncuts]->corte = vector<int>(gr.n_arcs - 1);
  // cuts[ncuts]->corte.aloca(gr.n_arcs -1);
  cuts[ncuts]->S[1] = v;

  for(i = 0, tam = 1; i < v; ++i)
    if(gr.nodes[i].in_W)
      cuts[ncuts]->S[++tam] = i;
  for(i = v + 1; i < n_vert; ++i)
    if(gr.nodes[i].in_W)
      cuts[ncuts]->S[++tam] = i;

  cuts[ncuts]->S[0] = tam;
  if(tam == 1) {
    _file <<"Gmindcut::add_cut - v: "<<v<<" tam: "<<tam<<" viol"<<viol<<" - corte nao gerado"<<std::endl;

    delete cuts[ncuts];
    cuts[ncuts] = NULL;
  } else {
  for(i = 0, tam = 0, val = 0; i < n_vert; ++i)
    if(gr.nodes[i].in_W == false) {
      aptr1 = gr.nodes[i].first_arc;
		  do {
				if(aptr1->adjac->in_W == true) {
          if(aptr1->cap > EPSILON)
			      val += aptr1->cap;
	    		cuts[ncuts]->corte[++tam] = aptr1->index;
	  		}
				aptr1 = aptr1->next;
      } while(aptr1 != gr.nodes[i].first_arc);
	  	} //  if( gr.nodes[i].in_W == true )
      cuts[ncuts]->corte[0] = tam;
      if(val > 1 - 2 * EPSILON) {
			  _file <<"Gmindcut::add_cut_dist - corte nao violado "<<val<<" "<<viol<<std::endl;
			  delete cuts[ncuts];
      } else ncuts++;
    }
}

bool Gmindcut::verif_corte(const double minc, const int v, const bool avisar) const {
  int i, n, tam;
  double undir = 0;//, un2 = 0;
  arc *aptr1;

  n = gr.n_nodes;
  for(i = 0; i < n; ++i)
    if(gr.nodes[i].in_W == false) {
      aptr1 = gr.nodes[i].first_arc;
			do {
		    if(aptr1->adjac->in_W)
					if(aptr1->cap >= EPSILON)
			    	undir += aptr1->cap;
		    aptr1 = aptr1->next;
		  } while(aptr1 != gr.nodes[i].first_arc);
  	}

  for(i = 0, tam = 0; i < n; ++i)
    if(gr.nodes[i].in_W) ++tam;

  if(undir > 1 - 10 * EPSILON) {
    if(avisar && (minc < undir - 1000 * EPSILON || minc > undir + 1000 * EPSILON ))
	  	_file <<"Gmindcut::verif_corte - undir e mincut - mincut: "<<minc<<" undir: "<<undir<<" v: "<<v<<" "<<gr.raiz-1<<" "<<tam<<std::endl;
    return false;
  } else if(minc > 1 - EPSILON && tam > 1 && avisar) {
		_file <<"Gmindcut::verif_corte - corte violado nao achado - minc: "<<minc<<" undir: "<<undir<<" v: "<<v<<std::endl;
		for(i = 0; i < gr.n_nodes; ++i)
		  if(gr.nodes[i].in_W) _file <<i<<" ";
		_file <<std::endl;
		// write_graph();
		exit(99);
  }

  return true;
}

void Gmindcut::generate_cuts(vector<vector<int> >& sol, int& ncuts, vector<str_cut*>& cuts, const bool only_best) {
  int i, j, k;

  if(only_best) del_cuts(ncuts, cuts);

  sol = vector<vector<int> >(ncuts);

  /*for(i = 0;i < ncuts; ++i) {
    sol[i] = vector<int>(cuts[i]->corte[0] + cuts[i]->S[0] + 1);
    ++ncut;
    for(j = 0; j <= cuts[i]->S[0]; ++j)
			sol[i][j] = cuts[i]->S[j];
    for(k = 1; k <= cuts[i]->corte[0]; ++j, ++k)
			sol[i][j] = cuts[i]->corte[k];
    delete cuts[i];
  }*/
  for(i = 0;i < ncuts; ++i) {
    sol[i] = vector<int>(cuts[i]->S[0]);
    ++ncut;
    for(j = 0; j < cuts[i]->S[0]; ++j)
			sol[i][j] = cuts[i]->S[j + 1];
    delete cuts[i];
  }
}

void Gmindcut::del_cuts(int& ncuts, vector<str_cut*>& cuts) const {
  int i;
  double viol = 0;
  bool apagar = false;
  const double m = 1;

  for(i = 0; i < ncuts; ++i)
    if(cuts[i]->viol >= viol + EPSILON) viol = cuts[i]->viol;

  for(i = 0; i < ncuts; ++i) {
    apagar = false;

    if(cuts[i]->viol <= m * viol - EPSILON)
  	apagar = true;
		// if(cuts[i]->viol[f] <= m*viol[f] - PRECISAO)

    if(apagar) {
  		delete cuts[i];
      cuts[i] = cuts[--ncuts];
		  cuts[ncuts] = NULL;
		  --i;
		}
  }
}
