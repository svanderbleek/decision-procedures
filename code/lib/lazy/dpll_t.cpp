#include <assert.h>

#include "dpll_t.h"

bool dpll_tt::is_satisfiable(const ast::nodet _f)
{
  f=_f;

  // first build skeleton
  skeleton(f, satcheck);
  
  // run SAT
  switch(satcheck.prop_solve())
  {
  case propt::P_SATISFIABLE:
    return true;
    
  case propt::P_UNSATISFIABLE:
    return false;
  
  case propt::P_ERROR:
  default:
    assert(false);
  }
}

void dpll_tt::theory_implication(const ast::nodet _n, bool value)
{
}
