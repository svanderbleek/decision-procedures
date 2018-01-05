#include <assert.h>

#include <iostream>

#include "parsing_context.h"

namespace parsing_context {

parsing_astt parsing_ast;
std::ostream *err=&std::cerr;

int parsing_astt::get_identifier(const dstring s)
{
  return get_type(s).token;
}

void parsing_astt::set_type(unsigned ids, unsigned t)
{
  const ast::_nodet &n=nodes[ids];

  if(n.id==N_LIST)
  {
    // recursion
    set_type(n.c1, t);
    set_type(n.c2, t);
  }
  else
  {
    bool is_predicate=nodes[t].id==N_BOOLEAN;
    type &T=get_type(n.string);
    T.node=t;
    T.token=is_predicate?N_PREDICATE:N_IDENTIFIER;
  }
}

}
