#include <stack>

#include "skeleton.h"

// converts the propositional skeleton
// and returns a vector of atoms

void skeletont::operator()(
  const ast::nodet root,
  propt &dest)
{
  // initialize nodes of skeleton graph
  nodes.resize(root.ast().number_of_nodes());

  // do axioms first
  convert_axioms(root.ast().root(), dest);
  
  // set the root to false
  set_to(root, false, dest);
}

literalt skeletont::convert_term(
  const ast::nodet n,
  propt &dest)
{
  nodes[n.number()].type=nodet::PROPOSITIONAL;
  nodes[n.number()].ast_node=n;

  literalt l;
  
  switch(n.id())
  {
  case N_PREDICATE:
    l=identifier_literal(n.string(), dest);
    break;

  // binary operators
  case N_CONJUNCTION:
  case N_DISJUNCTION:
  case N_BIIMPLICATION:
  case N_IMPLICATION:
    {
      literalt l1=convert_term(n.c1(), dest);
      literalt l2=convert_term(n.c2(), dest);

      switch(n.id())
      {
      case N_CONJUNCTION: l=dest.land(l1, l2); break;
      case N_DISJUNCTION: l=dest.lor(l1, l2); break;
      case N_BIIMPLICATION: l=dest.lequal(l1, l2); break;
      case N_IMPLICATION: l=dest.limplies(l1, l2); break;
      }
    }
    break;

  // the unary one
  case N_NEGATION:
    l=dest.lnot(convert_term(n.c1(), dest));
    break;

  // constants
  case N_TRUE:
    l=const_literal(true);
    break;
    
  case N_FALSE:
    l=const_literal(false);
    break;

  // everything else is a theory atom
  default:
    nodes[n.number()].type=nodet::THEORY;
    l=dest.new_variable();
  }

  nodes[n.number()].l=l;

  return l;
}

void skeletont::set_to(
  const ast::nodet n,
  bool value,
  propt &dest)
{
  nodes[n.number()].type=nodet::PROPOSITIONAL;
  nodes[n.number()].l=const_literal(value);
  nodes[n.number()].ast_node=n;
  
  switch(n.id())
  {
  // binary operators
  case N_CONJUNCTION:
    if(value)
    {
      set_to(n.c1(), value, dest);
      set_to(n.c2(), value, dest);
    }
    else
      dest.l_set_to(convert_term(n, dest), value);
    break;

  case N_DISJUNCTION:
    if(value)
    {
      bvt bv;
      bv.reserve(2);
      bv.push_back(convert_term(n.c1(), dest));
      bv.push_back(convert_term(n.c2(), dest));
      dest.lcnf(bv);
    }
    else
      dest.l_set_to(convert_term(n, dest), value);
    break;

  case N_NEGATION:
    set_to(n.c1(), !value, dest);
    break;

  // everything else goes to convert
  default:
    dest.l_set_to(convert_term(n, dest), value);
  }
}

void skeletont::convert_axioms(
  const ast::nodet n,
  propt &dest)
{
  switch(n.id())
  {
  case N_LIST:
    convert_axioms(n.c1(), dest);
    convert_axioms(n.c2(), dest);
    break;

  // binary operators
  case N_AXIOM:
    set_to(n.c2(), true, dest);
    break;

  default:;
  }
}

// dump assignment to Boolean variables in Skeleton

void skeletont::print_assignment(
  const propt &prop,
  std::ostream &out)
{
  for(unsigned i=0; i<identifier_map.size(); i++)
    if(identifier_map[i].is_set)
      std::cout << identifier_map[i].identifier << "=" 
                << prop.l_get(identifier_map[i].l)
                << std::endl;
}

literalt skeletont::identifier_literal(
  const dstring s,
  propt &dest)
{
  unsigned no=s.get_no();
  if(no>=identifier_map.size())
    identifier_map.resize(no+1);
  
  id_entryt &e=identifier_map[no];

  if(e.is_set) return e.l;
  e.is_set=true;
  e.l=dest.new_variable();
  e.identifier=s;
  return e.l;
}

void skeleton(const ast::nodet root, propt &dest)
{
  skeletont s;
  s(root, dest);
}
