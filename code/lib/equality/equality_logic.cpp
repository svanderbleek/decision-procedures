//
// Decision Procedures
// An Algorithmic Point of View
//
// Daniel Kroening, Ofer Strichman
//
// Equality Logic for a conjunction of equalities/disequalities
// using union find
//

#include "equality_logic.h"

void equality_logict::add_constraint(ast::nodet node, bool value)
{
  switch(node.id())
  {
  case N_CONJUNCTION:
    assert(value);
    add_constraint(node.c1(), true);
    add_constraint(node.c2(), true);
    break;

  case N_EQUAL:
    add_equality(node.c1(), node.c2(), value);
    break;

  case N_NOTEQUAL:
    add_equality(node.c1(), node.c2(), !value);
    break;

  case N_NEGATION:
    add_constraint(node.c1(), !value);
    break;

  default:
    assert(false);
  }
}

void equality_logict::add_equality(ast::nodet a, ast::nodet b, bool value)
{
  unsigned v_a=var_no(a);
  unsigned v_b=var_no(b);

  if(value)
    union_find.make_union(v_a, v_b);
  else
    diseq_list.push_back(diseqt(v_a, v_b));
}

bool equality_logict::get(ast::nodet node)
{
  return false;
}

void equality_logict::print_assignment(std::ostream &out)
{
  for(unsigned no=0; no<identifiers.size(); no++)
    if(identifiers[no])
    {
      dstring s;
      s.set(no);
      out << s << " = " << union_find.find(no) << std::endl;
    }
}

bool equality_logict::is_satisfiable(const ast::nodet _f)
{
  clear();
  add_constraint(_f, true);
  return is_consistent();
}

bool equality_logict::is_consistent()
{
  for(diseq_listt::const_iterator it=diseq_list.begin();
      it!=diseq_list.end();
      it++)
  {
    if(union_find.same_set(it->v1, it->v2))
      return false;
  }

  return true;
}


