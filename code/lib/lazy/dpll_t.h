//
// Decision Procedures
// An Algorithmic Point of View
//
// Daniel Kroening, Ofer Strichman
//
// DPLL(T)
//

#ifndef LAZY_DPLL_H
#define LAZY_DPLL_H

#include "../sat/satcheck.h"
#include "lazy_base.h"

class dpll_tt:public lazy_baset
{
public:
  virtual bool is_satisfiable(const ast::nodet _f);

protected:
  // the SAT solver
  satcheckt satcheck;

  // the decision procedure for the theory may communicate implications
  // back to the SAT solver
  virtual void theory_implication(ast::nodet node, bool value);
  
  // this is called when the solver backtracks
  // do learning within the theory
  virtual void explain()
  {
  }

  virtual propt &get_solver()
  {
    return satcheck;
  }
};

#endif
