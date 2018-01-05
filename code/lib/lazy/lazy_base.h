//
// Decision Procedures
// An Algorithmic Point of View
//
// Daniel Kroening, Ofer Strichman
//

#ifndef LAZY_BASE_H
#define LAZY_BASE_H

#include "../decision_procedure.h"
#include "../sat/skeleton.h"

class lazy_baset:public decision_proceduret
{
public:
  // main method
  virtual bool is_satisfiable(const ast::nodet _f)=0;

  virtual void print_assignment(std::ostream &out)
  {
    skeleton.print_assignment(get_solver(), out);
  }

  virtual bool get(const ast::nodet _f);
  
protected:
  // the formula
  ast::nodet f;

  // the skeleton
  skeletont skeleton;
  
  // this is called once the SAT solver assigns
  // a theory literal
  virtual void assignment(ast::nodet node, bool value)
  {
  }

  // check assignment for T-consistency
  // return true iff consistent
  virtual bool deduce()
  {
    return true;
  }

  // return reference to solver object
  virtual propt &get_solver()=0;
  
  void add_clause(const bvt &clause)
  {
    get_solver().lcnf(clause);
  }
};

#endif
