#include <assert.h>

#include "lazy_dpll.h"

bool lazy_dpllt::is_satisfiable(const ast::nodet _f)
{
  f=_f;
  number_of_iterations=0;

  // first build skeleton
  skeleton(f, satcheck);
  
  // run SAT
  while(true)
  {
    switch(satcheck.prop_solve())
    {
    case propt::P_SATISFIABLE:
      number_of_iterations++;

      if(deduce())
        return true;
      break;
      
    case propt::P_UNSATISFIABLE:
      return false;
    
    case propt::P_ERROR:
    default:
      assert(false);
    }
  }
}
