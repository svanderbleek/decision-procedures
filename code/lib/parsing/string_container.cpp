/*******************************************************************\

Module: Container for C-Strings

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#include <iostream>

#include <string.h>

#include "string_container.h"

/*******************************************************************\

Function: operator==

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

bool operator==(const string_ptrt a, const string_ptrt b)
{
  return strcmp(a.s, b.s)==0;
}

/*******************************************************************\

Function: hash_string

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

size_t hash_string(const char *s)
{
  size_t h=0;

  for(; *s!=0; s++)
    h=(h<<5)-h+*s;

  return h;
}

string_containert string_container;

