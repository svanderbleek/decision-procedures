//
// Decision Procedures
// An Algorithmic Point of View
//
// Daniel Kroening, Ofer Strichman
//
// Equality Logic for a conjunction of equalities/disequalities
// using union find
//

#ifndef EQUALITY_LOGIC_H
#define EQUALITY_LOGIC_H

#include <set>

#include "union_find.h"
#include "../decision_procedure.h"
#include "../parsing/ast.h"

class equality_logict:public decision_proceduret
{
public:
  virtual bool is_satisfiable(const ast::nodet _f);
  virtual void print_assignment(std::ostream &out);
  virtual bool get(const ast::nodet _f);

  void add_constraint(ast::nodet node, bool value);
  bool is_consistent();
  
protected:
  unsigned_union_find union_find;
  
  unsigned var_no(ast::nodet node)
  {
    assert(node.id()==N_IDENTIFIER);
    store_identifier(node.string());
    return node.string().get_no();
  }
  
  struct diseqt
  {
  public:
    unsigned v1, v2;
    
    diseqt(unsigned _v1, unsigned _v2):v1(_v1), v2(_v2)
    {
    }
  };
  
  typedef std::vector<diseqt> diseq_listt;
  diseq_listt diseq_list;

  typedef std::vector<bool> identifierst;
  identifierst identifiers;
  
  void store_identifier(const dstring &s)
  {
    unsigned no=s.get_no();
    if(no>=identifiers.size()) identifiers.resize(no+1);
    identifiers[no]=true;
  }
  
  void add_equality(ast::nodet a, ast::nodet b, bool value);
  
  void clear()
  {
    diseq_list.clear();
    identifiers.clear();
    union_find.clear();
  }
};

#endif
