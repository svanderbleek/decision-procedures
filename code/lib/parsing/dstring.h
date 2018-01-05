/*******************************************************************\

Module: Container for C-Strings

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#ifndef DSTRING_H
#define DSTRING_H

#include <iostream>

#include "string_container.h"

class dstring
{
public:
  dstring():no(0)
  {
  }

  dstring(const char *s):no(string_container[s])
  {
  }

  explicit dstring(const std::string &s):no(string_container[s.c_str()])
  {
  }

  friend bool operator < (const dstring a, const dstring b)
  {
    return a.no<b.no;
  }

  friend bool operator == (const dstring a, const dstring b)
  {
    return a.no==b.no;
  }
  
  friend bool operator != (const dstring a, const dstring b)
  {
    return a.no!=b.no;
  }
  
  unsigned get_no() const
  {
    return no;
  }
  
  void set(unsigned _no)
  {
    no=_no;
  }
  
  bool empty() const
  {
    return no==0;
  }
  
  const std::string &as_string() const
  {
    return string_container.string(get_no());
  }
  
protected:
  unsigned no;
};

bool operator < (const dstring a, const dstring b);
std::ostream &operator << (std::ostream &out, const dstring a);

#endif
