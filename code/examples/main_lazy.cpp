//
// Decision Procedures
// An Algorithmic Point of View
//
// Daniel Kroening, Ofer Strichman
//
// This example is an instance of a lazy
// encoding within a DPLL solver
//

#include <parsing/ast.h>
#include <lazy/lazy_dpll.h>
#include <equality/equality_logic.h>

class my_lazy_dpllt:public lazy_dpllt
{
protected:
  virtual bool deduce();
};

// check assignment for T-consistency
// return true iff consistent

bool my_lazy_dpllt::deduce()
{
  // iterate other theory atoms
  bvt clause;

  equality_logict equality_logic;

  for(skeletont::nodest::const_iterator
      it=skeleton.nodes.begin();
      it!=skeleton.nodes.end();
      it++)
    if(it->type==skeletont::nodet::THEORY)
    {
      bool value=get_solver().l_get(it->l).is_true();

      equality_logic.add_constraint(it->ast_node, value);

      clause.push_back(it->l.cond_negation(value));
    }

  if(equality_logic.is_consistent())
    return true;

  // block it
  add_clause(clause);

  return false;
}

int main(int argc, char **argv)
{
  if(argc!=2)
  {
    std::cerr << "expected file name as argument" << std::endl;
    return 1;
  }
  
  ast::astt ast;
  
  if(ast.parse(argv[1]))
  {
    std::cerr << "parsing failed" << std::endl;
    return 2;
  }

  std::cout << "Printing formula" << std::endl;
  std::cout << ast;
  
  ast::nodet f=ast.get_formula();
  
  if(f.is_nil())
  {
    std::cout << "No formula" << std::endl;
    return 3;
  }

  std::cout << "Solving formula" << std::endl;

  my_lazy_dpllt my_lazy_dpll;
  
  if(my_lazy_dpll.is_satisfiable(f))
  {
    std::cout << "*** SAT" << std::endl;
    my_lazy_dpll.print_assignment(std::cout);
  }
  else
    std::cout << "*** UNSAT" << std::endl;

  std::cout << "Iterations: " << my_lazy_dpll.get_number_of_iterations()
            << std::endl;
  
  return 0;
}
