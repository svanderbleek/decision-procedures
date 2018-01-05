//
// Decision Procedures
// An Algorithmic Point of View
//
// Daniel Kroening, Ofer Strichman
//
// Rational Numbers
//

#include <assert.h>

#include <string>
#include <sstream>
#include <iostream>

#include "rational.h"
#include "gcd.h"

/*******************************************************************\

Function: rationalt::normalize

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void rationalt::normalize()
{
  // first do sign

  if(denominator.is_negative())
  {
    denominator.negate();
    numerator.negate();
  }

  // divide by gcd

  integert _gcd=gcd(denominator, numerator);
  if(_gcd!=1 && !_gcd.is_zero())
  {
    denominator/=_gcd;
    numerator/=_gcd;
  }  
}

/*******************************************************************\

Function: rationalt::same_denominator

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void rationalt::same_denominator(rationalt &n)
{
  if(denominator==n.denominator) return;

  numerator*=n.denominator;
  n.numerator*=denominator;

  integert t=denominator*n.denominator;
  denominator=t;
  n.denominator=t;
}

/*******************************************************************\

Function: rationalt::invert

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void rationalt::invert()
{
  assert(numerator!=0);
  std::swap(numerator, denominator);
}

/*******************************************************************\

Function: inverse

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

rationalt inverse(const rationalt &n)
{
  rationalt tmp(n);
  tmp.invert();
  return tmp;
}

/*******************************************************************\

Function: operator<<

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

std::ostream& operator<<(std::ostream& out, const rationalt &a)
{
  std::ostringstream buf;
  buf << a.numerator;
  if(a.denominator!=1) buf << "/" << a.denominator;
  return out << buf.str();
}

/*******************************************************************\

Function: rational_from_string

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

rationalt rational_from_string(const std::string &s)
{
  unsigned i=0;
  rationalt result=0;

  // skip space
  for(; i<s.size() && s[i]==' ';  i++);

  for(; i<s.size() && isdigit(s[i]); i++)
  {
    unsigned n=s[i]-'0';
    assert(n<10);
    result*=10;
    result+=n;
  }

  // fractional part
  if(i<s.size() && s[i]=='.')
  {
    i++;

    rationalt fraction=1;
    rationalt fractional_part=0;

    for(; i<s.size() && isdigit(s[i]); i++)
    {
      unsigned n=s[i]-'0';
      assert(n<10);
      fraction*=10;
      fractional_part+=n*fraction;
    }

    result+=fractional_part;
  }

  return result;
}
