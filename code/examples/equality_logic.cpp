//
// Decision Procedures
// An Algorithmic Point of View
//
// Daniel Kroening, Ofer Strichman
//
// This example illustrates a
// decision procedure for equality logic based
// on union find
//

#include <stack>

#include <parsing/ast.h>
#include <equality/equality_logic.h>

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

  std::cout << "Passing formula to decision procedure" << std::endl;

  equality_logict equality_logic;
  
  if(equality_logic.is_satisfiable(f))
  {
    std::cout << "formula is satisfiable" << std::endl;
    equality_logic.print_assignment(std::cout);
  }
  else
    std::cout << "formula is unsatisfiable" << std::endl;

  return 0;
}
