//
// Decision Procedures
// An Algorithmic Point of View
//
// Daniel Kroening, Ofer Strichman
//
// Simplex
//

#ifndef GENERAL_SIMPLEX_H
#define GENERAL_SIMPLEX_H

#include "general_simplex_tableau.h"

class general_simplext:public general_simplex_tableaut
{
public:
  typedef enum { UNKNOWN, SAT, UNSAT } statust;

  general_simplext();
  statust solve();  

protected:
  statust setup_bounds();
  statust check();
  void update(variablet i, const rationalt &value);
  statust select(variablet &i);
  statust fix_lower_bound(variablet i);
  statust fix_upper_bound(variablet i);
  void pivot_and_update(variablet i, variablet j, const rationalt &v);
  void pivot(variablet i, variablet j);
};

#endif
