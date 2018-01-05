//
// Decision Procedures
// An Algorithmic Point of View
//
// Daniel Kroening, Ofer Strichman
//
// A lazy encoding using SAT-based splitting
//

#ifndef LAZY_DPLL_H
#define LAZY_DPLL_H

#include "lazy_base.h"
#include "../sat/satcheck.h"

class lazy_dpllt:public lazy_baset
{
public:
  // main method
  virtual bool is_satisfiable(const ast::nodet _f);
  
  unsigned get_number_of_iterations() const
  {
    return number_of_iterations;
  }
  
protected:
  // the solver
  satcheckt satcheck;
  
  virtual propt &get_solver()
  {
    return satcheck;
  }

  unsigned number_of_iterations;
};

#endif
