/*******************************************************************\

Module:

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#include <assert.h>

#include "prop.h"

/*******************************************************************\

Function: propt::set_equal

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void propt::set_equal(literalt a, literalt b)
{
  bvt bv;
  bv.resize(2);
  bv[0]=a;
  bv[1]=lnot(b);
  lcnf(bv);
  bv[0]=lnot(a);
  bv[1]=b;
  lcnf(bv);
}

/*******************************************************************\

Function: propt::set_assignment

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void propt::set_assignment(literalt a, bool value)
{
  assert(false);
}

/*******************************************************************\

Function: propt::copy_assignment_from

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void propt::copy_assignment_from(const propt &src)
{
  assert(false);
}

/*******************************************************************\

Function: propt::is_in_core

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

bool propt::is_in_core(literalt l) const
{
  assert(false);
  return false;
}
