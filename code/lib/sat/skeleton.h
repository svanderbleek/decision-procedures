#ifndef SKELETON_H
#define SKELETON_H

#include "../parsing/ast.h"
#include "prop.h"

// converts the propositional skeleton
// and retuns a vector of atoms

class skeletont
{
public:
  void operator()(const ast::nodet root, propt &dest);
  
  class nodet
  {
  public:
    typedef enum { NONE, PROPOSITIONAL, THEORY } typet;
    typet type;
  
    nodet():type(NONE)
    {
    }

    literalt l;
    ast::nodet ast_node;
  };

  typedef std::vector<nodet> nodest;
  nodest nodes;
  
  // print assignment to Boolean variables  
  void print_assignment(
    const propt &prop,
    std::ostream &out);

protected:
  class id_entryt
  {
  public:
    literalt l;
    bool is_set;
    dstring identifier;
    
    id_entryt():is_set(false)
    {
    }
  };
  
  typedef std::vector<id_entryt> identifier_mapt;
  identifier_mapt identifier_map;
  
  literalt identifier_literal(
    const dstring s,
    propt &dest);

  literalt convert_term(ast::nodet n, propt &dest);
  
  void convert_axioms(const ast::nodet n, propt &dest);
  void set_to(const ast::nodet n, bool value, propt &dest);
};

void skeleton(const ast::nodet root, propt &dest);

#endif
