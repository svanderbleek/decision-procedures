//
// Decision Procedures
// An Algorithmic Point of View
//
// Daniel Kroening, Ofer Strichman
//
// This example illustrates the conversion of a Boolean
// formula into CNF using Tseitin's encoding
//

#include <parsing/ast.h>
#include <sat/skeleton.h>
#include <sat/dimacs_cnf.h>

int main(int argc, char **argv)
{
  if(argc!=2)
  {
    std::cerr << "expected file name as argument" << std::endl;
    return 1;
  }
  
  ast::astt ast;

  // undeclared variables are predicates
  ast.set_default_id_token(N_PREDICATE);
  
  if(ast.parse(argv[1]))
  {
    std::cerr << "parsing failed" << std::endl;
    return 2;
  }

  ast::nodet f=ast.get_formula();
  
  if(f.is_nil())
  {
    std::cout << "No formula" << std::endl;
    return 3;
  }

  dimacs_cnft dimacs_cnf;
  skeleton(f, dimacs_cnf);

  dimacs_cnf.write_dimacs_cnf(std::cout);

  return 0;
}
