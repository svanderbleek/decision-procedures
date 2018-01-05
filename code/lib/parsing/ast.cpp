#include <iostream>

#include "ast.h"
#include "parsing_context.h"

bool ast::astt::parse(const std::string &file)
{
  parsing_context::parsing_ast.set_default_id_token(default_id_token);

  bool result=parsing_context::parse_file(file);

  if(!result)
    swap(parsing_context::parsing_ast);

  return result;
}

ast::nodet ast::nil_node()
{
  return ast::nodet(0, 0);
}

std::ostream &ast::operator << (std::ostream &out, const nodet &node)
{
  node.output(out, false);
  return out;
}

std::ostream &ast::operator << (std::ostream &out, const astt &ast)
{
  ast.output(out);
  return out;
}

void ast::astt::output(std::ostream &out) const
{
  if(nodes.empty()) return;
  root().output(out, false);
}

ast::nodet ast::astt::get_type_node(const dstring s) const
{
  unsigned no=s.get_no();
  if(no>=id_type_map.size()) return nodet(0, this);
  return nodet(id_type_map[no].node, this);
}

void ast::nodet::output(std::ostream &out, bool comma_sep) const
{
  if(id()==N_LIST)
  {
    c1().output(out, comma_sep);
    if(comma_sep) out << ", ";
    c2().output(out, comma_sep);
  }
  else
  {
    if(_n().c1!=0 && _n().c2!=0)
    {
      out << "[";
      c1().output(out, true);
      out << " ";
      _n().output(out);
      out << " ";
      c2().output(out, true);
      out << "]";
    }
    else if(_n().c1!=0)
    {
      _n().output(out);
      out << " ";
      c1().output(out, true);
    }
    else if(_n().c2!=0)
    {
      _n().output(out);
      out << " ";
      c2().output(out, true);
    }
    else
      _n().output(out);
    
    if(!comma_sep) out << std::endl;
  }
}

void ast::_nodet::output(std::ostream &out) const
{
  switch(id)
  {
  case END: out << "END"; break;
  case N_IDENTIFIER: out << "identifier '" << string << "'"; break;
  case N_INTEGER: out << "integer '" << string << "'"; break;
  case N_RATIONAL: out << "rational '" << string << "'"; break;
  case N_INT: out << "int"; break;
  case N_REAL: out << "real"; break;
  case N_BOOLEAN: out << "boolean"; break;
  case N_UNSIGNED: out << "unsigned"; break;
  case N_SIGNED: out << "signed"; break;
  case N_AXIOM: out << "axiom"; break;
  case N_DECLARATION: out << "declaration"; break;
  case N_THEOREM: out << "theorem"; break;
  case N_CONJUNCTION: out << "&"; break;
  case N_DISJUNCTION: out << "|"; break;
  case N_NEGATION: out << "!"; break;
  case N_BIIMPLICATION: out << "<=>"; break;
  case N_IMPLICATION: out << "=>"; break;
  case N_TRUE: out << "true"; break;
  case N_FALSE: out << "false"; break;
  case N_ADDITION: out << "+"; break;
  case N_SUBTRACTION: out << "-"; break;
  case N_MULTIPLICATION: out << "*"; break;
  case N_DIVISION: out << "/"; break;
  case N_MODULO: out << "%"; break;
  case N_UMINUS: out << "UMINUS"; break;
  case N_LOWER: out << "<"; break;
  case N_GREATER: out << ">"; break;
  case N_LOWEREQUAL: out << "<="; break;
  case N_GREATEREQUAL: out << ">="; break;
  case N_EQUAL: out << "="; break;
  case N_FORALL: out << "forall"; break;
  case N_EXISTS: out << "exists"; break;
  case N_LIST: out << "LIST"; break;
  case N_PREDICATE: out << "predicate '" << string << "'"; break;
  case N_FUNCTION: out << "function"; break;
  default: out << "?";
  }
}

const ast::nodet ast::astt::root() const
{
  return ast::nodet(nodes.size()-1, this);
}

ast::nodet ast::astt::get_formula() const
{
  return get_formula(root());
}

ast::nodet ast::astt::get_formula(const ast::nodet n) const
{
  switch(n.id())
  {
  case N_LIST:
    {
      ast::nodet t;
      t=get_formula(n.c1());
      if(!t.is_nil()) return t;
    }
    return get_formula(n.c2());
  
  case N_THEOREM:
    return n.c2();
  
  default:
    return ast::nil_node();
  }
}

