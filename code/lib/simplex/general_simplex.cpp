//
// Decision Procedures
// An Algorithmic Point of View
//
// Daniel Kroening, Ofer Strichman
//
// General Simplex
//

#define DEBUG

#include "general_simplex.h"

general_simplext::general_simplext()
{
}

general_simplext::statust general_simplext::setup_bounds()
{
  #ifdef DEBUG
  std::cout << "SETTING UP BOUNDS" << std::endl;
  #endif

  for(unsigned v=0; v<variables.size(); v++)
  {
    value(variablet(v))=0;
    if(variables[v].bound_is_unsat())
      return UNSAT;
  }

  // fix values for non-basic variables
  variablet v;
  for(v.index=0; v.index<variables.size(); v.index++)
  {
    vart &var=variables[v.index];

    #ifdef DEBUG
    std::cout << "SETUP " << get_variable_name(v) << std::endl;
    #endif

    if(!var.basic)
    {
      if(var.violates_lower())
      {
        #ifdef DEBUG
        std::cout << get_variable_name(v) << " violates lower" << std::endl;
        #endif

        update(v, var.lower_bound.value);
      }
      else if(var.violates_upper())
      {
        #ifdef DEBUG
        std::cout << get_variable_name(v) << " violates upper" << std::endl;
        #endif

        update(v, var.upper_bound.value);
      }
    }
  }

  return UNKNOWN;
}

void general_simplext::update(variablet i, const rationalt &v)
{
  vart &i_var=variables[i.index];

  // i must be non-basic variable -- get the column of i
  column_indt i_column=get_column(i);

  // iterate over the basic variables (i.e., rows)
  // this should be optimized with a row-index-list
  for(tableaut::const_iterator
      r_it=tableau.begin();
      r_it!=tableau.end();
      r_it++)
  {
    const rowt &row=*r_it;
    value(row.variable)+=
      row.get_coefficient(i_column)*
      (v-i_var.value);
  }

  // set new value
  i_var.value=v;
}

general_simplext::statust general_simplext::solve()
{
  if(setup_bounds()==UNSAT) return UNSAT;

  return check();
}

general_simplext::statust general_simplext::select(variablet &i)
{
  // smallest basic variable that is out-of-bounde
  // this should be optimized with a set that keeps those variables

  for(i.index=0; i.index<variables.size(); i.index++)
  {
    vart &var=variables[i.index];
    if(var.basic && !var.in_bounds())
       return UNKNOWN;
  }

  return SAT;
}

general_simplext::statust general_simplext::check()
{
  while(true)
  {
    #ifdef DEBUG
    std::cout << *this << std::endl;
    #endif

    variablet i;

    if(select(i)==SAT) return SAT;

    #ifdef DEBUG
    std::cout << "SELECTED BASIC " << get_variable_name(i) << "("
              << i.index << ")" << std::endl;
    #endif

    vart &i_var=variables[i.index];

    if(i_var.violates_lower())
    {
      #ifdef DEBUG
      std::cout << "VIOLATES LOWER" << std::endl;
      #endif

      if(fix_lower_bound(i)==UNSAT)
        return UNSAT;
    }
    else if(i_var.violates_upper())
    {
      #ifdef DEBUG
      std::cout << "VIOLATES UPPER" << std::endl;
      #endif

      if(fix_upper_bound(i)==UNSAT)
        return UNSAT;
    }
    else
      assert(false);
  }
}

general_simplext::statust general_simplext::fix_lower_bound(variablet i)
{
  #ifdef DEBUG
  std::cout << "FIXING LOWER BOUND, ROW: " << get_row(i).index << std::endl;
  #endif

  const rowt &row=tableau[get_row(i).index];

  // get a non-basic variable that allows us to move
  for(rowt::coefficientst::const_iterator
      c_it=row.coefficients.begin();
      c_it!=row.coefficients.end();
      c_it++)
  {
    const rationalt &a_ij=c_it->second;
    variablet j=variable(c_it->first);
    const vart &j_var=variables[j.index];

    if((a_ij>0 && !j_var.upper_bound_is_tight()) ||
       (a_ij<0 && !j_var.lower_bound_is_tight()))
    {
      pivot_and_update(i, j, lower_bound(i));
      return UNKNOWN;
    }
  }

  return UNSAT;
}

general_simplext::statust general_simplext::fix_upper_bound(variablet i)
{
  #ifdef DEBUG
  std::cout << "FIXING UPPER BOUND, ROW: " << get_row(i).index << std::endl;
  #endif

  const rowt &row=tableau[get_row(i).index];

  // get a non-basic variable that allows us to move
  for(rowt::coefficientst::const_iterator
      c_it=row.coefficients.begin();
      c_it!=row.coefficients.end();
      c_it++)
  {
    const rationalt &a_ij=c_it->second;
    variablet j=variable(c_it->first);
    const vart &j_var=variables[j.index];

    if((a_ij<0 && !j_var.upper_bound_is_tight()) ||
       (a_ij>0 && !j_var.lower_bound_is_tight()))
    {
      pivot_and_update(i, j, upper_bound(i));
      return UNKNOWN;
    }
  }

  return UNSAT;
}

void general_simplext::pivot_and_update(
  variablet i, // basic
  variablet j, // non-basic
  const rationalt &v)
{
  #ifdef DEBUG
  std::cout << "PIVOTING " << get_variable_name(i) << " (basic) and "
            << get_variable_name(j) << " (non-basic)" << std::endl;
  #endif

  rationalt theta=(v-value(i))/get_coefficient(i, j);

  value(i)=v;
  value(j)+=theta;

  #ifdef DEBUG  
  std::cout << "THETA: " << theta << std::endl;
  #endif

  // j must be non-basic variable -- get the column of j
  column_indt j_column=get_column(j);

  // i must be basic variable -- get the row of i
  row_indt i_row=get_row(i);

  // iterate over the basic variables (i.e., rows)
  // this should be optimized with a row-index-list
  for(tableaut::const_iterator
      r_it=tableau.begin();
      r_it!=tableau.end();
      r_it++)
    if(r_it->variable!=i)
    {
      const rowt &row=*r_it;
      value(row.variable)+=row.get_coefficient(j_column)*theta;
    }

  // pivot i, j
  pivot(i, j);
}

void general_simplext::pivot(
  variablet i, // basic
  variablet j) // non-basic
{
  rationalt a_ij=get_coefficient(i, j);

  assert(!a_ij.is_zero());

  // divide the i-row by a_ij
  row_indt i_row_ind=get_row(i);
  
  #ifdef DEBUG
  std::cout << "a_ij = " << a_ij << std::endl;
  #endif

  rowt &i_row=tableau[i_row_ind.index];
  column_indt j_column_ind=get_column(j);
  
  for(rowt::coefficientst::iterator
      c_it=i_row.coefficients.begin();
      c_it!=i_row.coefficients.end();
      c_it++)
    c_it->second/=-a_ij;

  // use this row to eliminate j
  // iterate over the basic variables (i.e., rows)
  // this should be optimized with a row-index-list
  for(tableaut::iterator
      r_it=tableau.begin();
      r_it!=tableau.end();
      r_it++)
    if(r_it->variable!=i)
    {
      rowt &other_row=*r_it;
      
      rationalt factor=
        other_row.get_coefficient(j_column_ind);
        
      #ifdef DEBUG
      std::cout << "FACTOR: " << factor << std::endl;
      #endif
        
      for(rowt::coefficientst::iterator
          c_it=i_row.coefficients.begin();
          c_it!=i_row.coefficients.end();
          c_it++)
        other_row[c_it->first]+=c_it->second*factor;

      rationalt &other_j=other_row[j_column_ind];
      assert(other_j.is_zero());
      other_j=(factor/a_ij);
    }
    
  i_row[j_column_ind]=1/a_ij;
    
  // do the swap

  std::swap(tableau[i_row_ind.index].variable,
            columns[j_column_ind.index].variable);

  variables[i.index].make_non_basic(j_column_ind);
  variables[j.index].make_basic(i_row_ind);
}
