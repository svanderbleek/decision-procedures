/*******************************************************************\

Module:

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#include <assert.h>

#include "gcd.h"

/*******************************************************************\

Function: gcd

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

integert gcd(const integert &_a, const integert &_b)
{
  integert a(_a), b(_b);

  if(a.is_negative()) a.negate();
  if(b.is_negative()) b.negate();

  if(a==1) return a;
  if(b==1) return b;
  if(a==b) return a;

  while(!b.is_zero())
  {
    integert tmp_b(b);
    b=a%b;
    a=tmp_b;
  }

  assert(!a.is_negative());

  return a;
}
