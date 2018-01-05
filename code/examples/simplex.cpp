//
// Decision Procedures
// An Algorithmic Point of View
//
// Daniel Kroening, Ofer Strichman
//
// This example illustrates how to apply
// the simplex method
//

#include <stack>

#include <parsing/ast.h>
#include <simplex/linear_constraint.h>
#include <simplex/general_simplex.h>

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

  std::cout << "Passing formula to simplex" << std::endl;
  
  general_simplext simplex;

  add_linear_constraint(f, simplex);

  std::cout << "Solving" << std::endl;

  general_simplext::statust status=simplex.solve();

  std::cout << "Formula is "
            << (status==general_simplext::SAT?"feasible":"infeasible")
            << std::endl;

  return 0;
}
