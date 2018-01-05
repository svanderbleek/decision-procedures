//
// Decision Procedures
// An Algorithmic Point of View
//
// Daniel Kroening, Ofer Strichman
//
// General Simplex Tableau
//

#include <iomanip>

#include "general_simplex_tableau.h"

std::ostream &operator<<(std::ostream &out, const general_simplex_tableaut &s)
{
  s.print_tableau(out);
  return out;
}

general_simplex_tableaut::general_simplex_tableaut()
{
  auxiliary_variable_counter=0;
}

dstring general_simplex_tableaut::get_variable_name(variablet v) const
{
  if(!variables[v.index].name.empty())
    return variables[v.index].name;

  char buf[100];
  snprintf(buf, 99, "x%u", v.index);
  return dstring(buf);
}

void general_simplex_tableaut::rowt::negate()
{
  for(coefficientst::iterator
      c_it=coefficients.begin();
      c_it!=coefficients.end();
      c_it++)
    c_it->second.negate();
}

general_simplex_tableaut::rowt &general_simplex_tableaut::new_auxiliary_variable()
{
  variablet v=new_variable();
  tableau.push_back(rowt());
  tableau.back().variable=v;
  vart &var=variables[v.index];
  var.make_basic(row_indt(tableau.size()-1));
  var.column=column_indt(unsigned(-1));
  return tableau.back();
}

general_simplex_tableaut::variablet general_simplex_tableaut::map_variable(dstring s)
{
  unsigned n=s.get_no();

  if(n>=variable_map.size())
  variable_map.resize(n+1, variablet(unsigned(-1)));

  if(variable_map[n].index!=unsigned(-1))
  {
    variablet v=variable_map[n];
    assert(!variables[v.index].basic);
    return v;
  }

  variablet v=new_variable();
  variable_map[n]=v;
  variables[v.index].name=s;
  variables[v.index].make_non_basic(column_indt(columns.size()));

  columns.push_back(columnt());
  columns.back().variable=v;

  assert(!variables[v.index].basic);

  return v;
}

void general_simplex_tableaut::print_tableau(std::ostream &out) const
{
  out << std::setw(5) << "";

  for(unsigned c=0; c<columns.size(); c++)
  {
    out << " |";
    out << std::setw(5) << get_variable_name(columns[c].variable);
  }

  out << std::endl;

  for(unsigned c=0; c<columns.size(); c++)
    out << "------|";

  out << "------";
  out << std::endl;

  unsigned row_number=0;

  for(tableaut::const_iterator
      r_it=tableau.begin();
      r_it!=tableau.end();
      r_it++, row_number++)
  {
    out << std::setw(5) << get_variable_name(r_it->variable);

    column_indt c;
    rationalt sum=0;
    for(c.index=0; c.index<columns.size(); c.index++)
    {
      out << " |";
      rationalt f=r_it->get_coefficient(c);
      out << std::setw(5) << f;
      sum+=f*value(variable(c));
    }

    out << " = " << std::setw(5) << sum;

    out << std::endl;
  }

  out << std::endl;

  for(unsigned v=0; v<variables.size(); v++)
  {
    const vart &var=variables[v];

    if(var.basic)
    {
      out << "B (row " << std::setw(3) << var.row.index << ") ";
    }
    else
    {
      out << "N (col " << std::setw(3) << var.column.index << ") ";
    }

    if(var.lower_bound.is_set) 
      out << std::setw(5) << var.lower_bound.value << " <= ";
    else
      out << std::setw(5) << "" << "    ";

    out << std::setw(5) << get_variable_name(variablet(v));
    out << "=" << std::setw(5) << var.value;

    if(var.upper_bound.is_set)
      out << " <= " << std::setw(5) << var.upper_bound.value;

    out << std::endl;
  }
}

void general_simplex_tableaut::print_tableau_latex(std::ostream &out) const
{
}

void general_simplex_tableaut::print_tableau_formulas(std::ostream &out) const
{
}

void general_simplex_tableaut::compute_rows()
{
  #if 0
  for(tableaut::iterator
      r_it=tableau.begin();
      r_it!=tableau.end();
      r_it++)
  {
    rowt &row=*r_it;

    for(rowt::coefficientst::const_iterator
        c_it=row.coefficients.begin();
        c_it!=row.coefficients.end();
        c_it++)
      if(!c_it->second.is_zero())
        variables[c_it->first].rows.insert(&row);
  }
  #endif
}

void general_simplex_tableaut::add_lower_bound(variablet variable, rationalt value)
{
  vart &var=variables[variable.index];
  if(var.lower_bound.is_set)
  {
    if(var.lower_bound.value>value) var.lower_bound.value=value;
  }
  else
    var.lower_bound.set(value);
}

void general_simplex_tableaut::add_upper_bound(variablet variable, rationalt value)
{
  vart &var=variables[variable.index];
  if(var.upper_bound.is_set)
  {
    if(var.upper_bound.value<value) var.upper_bound.value=value;
  }
  else
    var.upper_bound.set(value);
}

