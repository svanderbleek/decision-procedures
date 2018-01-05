#ifndef AST_NODE_H
#define AST_NODE_H

#include <map>
#include <vector>
#include <stack>

#include "dstring.h"
#include "parser.hpp"

namespace ast {

class _nodet
{
public:
  class visitor;

  _nodet():id(0), c1(0), c2(0)
  {
  }

  explicit _nodet(int _id):id(_id), c1(0), c2(0)
  {
  }

  _nodet(int _id, const dstring _string):id(_id), c1(0), c2(0), string(_string)
  {
  }

  _nodet(int _id, unsigned _c1):id(_id), c1(_c1), c2(0)
  {
  }

  _nodet(int _id, unsigned _c1, unsigned _c2):id(_id), c1(_c1), c2(_c2)
  {
  }

  // node label
  int id;
  
  // children
  unsigned c1, c2;
  
  // possible string ref.
  dstring string;
  
  void output(std::ostream &out) const;
};

class nodet;

class astt
{
public:
  friend class nodet;
  
  astt():default_id_token(N_IDENTIFIER)
  {
    // make dummy 0 node
    nodes.push_back(_nodet());
  }

  unsigned number_of_nodes() const
  {
    return nodes.size();
  }
  
  unsigned new_node()
  {
    nodes.push_back(_nodet());
    return nodes.size()-1;
  }
  
  unsigned new_node(int id)
  {
    nodes.push_back(_nodet(id));
    return nodes.size()-1;
  }
  
  unsigned new_node(int id, const dstring s)
  {
    nodes.push_back(_nodet(id, s));
    return nodes.size()-1;
  }
  
  unsigned new_node(int id, unsigned c1)
  {
    nodes.push_back(_nodet(id, c1));
    return nodes.size()-1;
  }
  
  unsigned new_node(int id, unsigned c1, unsigned c2)
  {
    nodes.push_back(_nodet(id, c1, c2));
    return nodes.size()-1;
  }
  
  _nodet &operator[](unsigned n)
  {
    return nodes[n];
  }

  const _nodet &operator[](unsigned n) const
  {
    return nodes[n];
  }
  
  const class nodet root() const;
  
  bool parse(const std::string &file);
  
  void output(std::ostream &out) const;

  void swap(astt &other)
  {
    other.nodes.swap(nodes);
    other.id_type_map.swap(id_type_map);
    std::swap(default_id_token, other.default_id_token);
  }
  
  void clear()
  {
    nodes.clear();
    id_type_map.clear();
  }
  
  class nodet get_type_node(const dstring s) const;
  class nodet get_formula() const;
  
  void set_default_id_token(int t)
  {
    default_id_token=t;
  }

protected:
  typedef std::vector<_nodet> nodest;
  nodest nodes;

  class type
  {
  public:
    unsigned node;
    int token;
    
    type():node(0), token(0)
    {
    }
  };

  typedef std::vector<type> id_type_mapt;
  id_type_mapt id_type_map;
  int default_id_token;
  
  type &get_type(dstring s)
  {
    unsigned no=s.get_no();

    if(id_type_map.size()<=no)
      id_type_map.resize(no+1);

    if(id_type_map[no].token==0)
       id_type_map[no].token=default_id_token;

    return id_type_map[no];
  }

  class nodet get_formula(const ast::nodet n) const;
  
public:
  const id_type_mapt &get_id_type_map() const
  {
    return id_type_map;
  }
};

// node abstraction
class nodet
{
public:
  nodet(): __ast(0)
  {
  }
  
  nodet(unsigned _number, const class astt *_ast):
    __number(_number), __ast(_ast)
  {
  }
  
  int id() const
  {
    return _n().id;
  }

  const dstring string() const
  {
    return _n().string;
  }

  const nodet c1() const
  {
    return nodet(_n().c1, __ast);
  }

  const nodet c2() const
  {
    return nodet(_n().c2, __ast);
  }
  
  unsigned number() const
  {
    return __number;
  }
  
  void output(std::ostream &out, bool comma_sep) const;
  
  bool is_nil() const
  {
    return __number==0;
  }
  
  const class astt &ast() const
  {
     return *__ast;
  }

protected:
  unsigned __number;
  const class astt *__ast;
  
  const _nodet _n() const
  {
    return (*__ast)[__number];
  }
};

nodet nil_node();

std::ostream &operator << (std::ostream &, const nodet &);
std::ostream &operator << (std::ostream &, const astt &);

}

#endif
