//
// Decision Procedures
// An Algorithmic Point of View
//
// Daniel Kroening, Ofer Strichman
//
// Simplex
//

#include "linear_constraint.h"

class linear_constraintt
{
public:
  linear_constraintt(general_simplex_tableaut &_tableau):tableau(_tableau)
  {
  }

  void add(ast::nodet f);

protected:
  general_simplex_tableaut &tableau;

  static rationalt convert_factor(ast::nodet f);

  void convert_product(ast::nodet p, bool negate);
  void convert_term(ast::nodet p, bool negate);
  void collect_products(ast::nodet n);
  void convert_atom(ast::nodet atom);

  typedef general_simplex_tableaut::rowt::coefficientst productst;
  productst products;
  rationalt constant;
};

rationalt linear_constraintt::convert_factor(ast::nodet f)
{
  assert(!f.is_nil());

  if(f.id()==N_INTEGER || f.id()==N_RATIONAL)
    return rational_from_string(f.string().as_string());
  else if(f.id()==N_UMINUS)
    return -convert_factor(f.c1());
  else
  {
    std::cerr << "Not a linear factor: " << f << std::endl;
    exit(1);
  }
}

void linear_constraintt::convert_product(ast::nodet p, bool negate)
{
  if(p.id()==N_INTEGER ||
     p.id()==N_RATIONAL ||
     p.id()==N_UMINUS && (p.c1().id()!=N_IDENTIFIER))
  {
    // this goes into the constant
    rationalt c=convert_factor(p);
    if(negate) c.negate();
    constant-=c;
    return;
  }

  // this goes into the product list
  rationalt factor=1;
  ast::nodet identifier=ast::nil_node();

  if(p.id()==N_MULTIPLICATION)
  {
    ast::nodet constant=ast::nil_node();

    if(p.c1().id()==N_IDENTIFIER)
    {
      identifier=p.c1();
      constant=p.c2();
    }
    else
    {
      constant=p.c1();
      identifier=p.c2();
    }

    assert(!identifier.is_nil());
    assert(!constant.is_nil());

    factor=convert_factor(constant);
  }
  else if(p.id()==N_UMINUS)
  {
    negate=!negate;
    identifier=p.c1();
  }
  else
    identifier=p;

  if(identifier.id()!=N_IDENTIFIER)
  {
    std::cerr << "Not a linear term: " << p << std::endl;
    exit(1);
  }  

  general_simplex_tableaut::variablet var=
    tableau.map_variable(identifier.string());

  general_simplex_tableaut::column_indt column=
    tableau.get_column(var);

  if(negate) factor.negate();

  products[column]+=factor;
}

void linear_constraintt::convert_term(ast::nodet term, bool negate)
{
  assert(!term.is_nil());

  switch(term.id())
  {
  case N_ADDITION:
    convert_term(term.c1(), negate);
    convert_term(term.c2(), negate);
    break;

  case N_SUBTRACTION:
    convert_term(term.c1(), negate);
    convert_term(term.c2(), !negate);
    break;

  default:
    convert_product(term, negate);
  }  
}

void linear_constraintt::collect_products(ast::nodet constraint)
{
  if(constraint.id()!=N_LOWEREQUAL &&
     constraint.id()!=N_GREATEREQUAL &&
     constraint.id()!=N_EQUAL)
  {
    std::cerr << "not a linear term: " << constraint << std::endl;
    exit(1);
  }

  constant=0;
  products.clear();

  convert_term(constraint.c1(), true);
  convert_term(constraint.c2(), false);

  // remove zero entries
  for(productst::iterator
      it=products.begin();
      it!=products.end();
      ) // no it++
    if(it->second.is_zero())
    {
      productst::iterator next=it;
      next++;
      products.erase(it);
      it=next;
    }
    else
      it++;
}

void linear_constraintt::convert_atom(ast::nodet atom)
{
  assert(!atom.is_nil());

  collect_products(atom);

  general_simplex_tableaut::variablet variable;
  int id=atom.id();
  bool flip=false;

  // catch simple bounds here
  if(products.empty()) // this is zero
  {

    return;
  }

  if(products.size()==1)
  {
    // this is really a bound
    general_simplex_tableaut::column_indt column=
      products.begin()->first;

    variable=tableau.variable(column);
    constant/=products.begin()->second;

    // flip if negative
    if(products.begin()->second.is_negative())
      flip=!flip;
  }
  else
  {
    general_simplex_tableaut::rowt &row=
      tableau.new_auxiliary_variable();

    row.coefficients.swap(products);
    variable=row.variable;
    
    // normalize a bit: flip if the constant is negative
    // if it's zero, flip on first constraint
    if(constant.is_negative() ||
       (constant.is_zero() &&
        row.coefficients.begin()->second.is_negative()))
    {
      flip=!flip;
      row.negate();
      constant.negate();
    }
  }

  if(flip)
  {
    if(id==N_LOWEREQUAL)
      id=N_GREATEREQUAL;
    else if(id==N_GREATEREQUAL)
      id=N_LOWEREQUAL;
  }

  switch(id)
  {
  case N_LOWEREQUAL:
    tableau.add_lower_bound(variable, constant);
    break;

  case N_GREATEREQUAL:
    tableau.add_upper_bound(variable, constant);
    break;

  case N_EQUAL:
    tableau.add_lower_bound(variable, constant);
    tableau.add_upper_bound(variable, constant);
    break;

  default:
    assert(false);
  }
}

void linear_constraintt::add(ast::nodet root)
{
  std::stack<ast::nodet> conjuncts;

  conjuncts.push(root);

  while(!conjuncts.empty())
  {
    ast::nodet n=conjuncts.top();
    conjuncts.pop();

    if(n.id()==N_CONJUNCTION)
    {
      // put on stack
      conjuncts.push(n.c2());
      conjuncts.push(n.c1());
    }
    else
      convert_atom(n);
  }
}

void add_linear_constraint(
  ast::nodet root,
  general_simplex_tableaut &tableau)
{
  linear_constraintt linear_constraint(tableau);
  linear_constraint.add(root);
}

