//
// Decision Procedures
// An Algorithmic Point of View
//
// Daniel Kroening, Ofer Strichman
//

#ifndef DECISION_PROCEDURE_H
#define DECISION_PROCEDURE_H

#include "parsing/ast.h"

// interface definition for decision procedures

class decision_proceduret
{
public:
  virtual ~decision_proceduret()
  {
  }

  // main method
  virtual bool is_satisfiable(const ast::nodet _f)=0;

  // satisfying assignment
  virtual void print_assignment(std::ostream &out)=0;
  virtual bool get(const ast::nodet _f)=0;
};

#endif
