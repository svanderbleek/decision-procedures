//
// Decision Procedures
// An Algorithmic Point of View
//
// Daniel Kroening, Ofer Strichman
//
// Simplex
//

#ifndef LINEAR_CONSTRAINT_H
#define LINEAR_CONSTRAINT_H

#include "../parsing/ast.h"
#include "general_simplex_tableau.h"

void add_linear_constraint(
  ast::nodet f,
  general_simplex_tableaut &tableau);

#endif
