//
// Decision Procedures
// An Algorithmic Point of View
//
// Daniel Kroening, Ofer Strichman
//
// Simplex Matrix
//

#ifndef GENERAL_SIMPLEX_MATRIX_H
#define GENERAL_SIMPLEX_MATRIX_H

#include <map>
#include <list>
#include <set>
#include <iostream>

#include "../parsing/dstring.h"
#include "rational.h"

class general_simplex_tableaut
{
public:
  // this data-structure is optimized for very sparse matrices
  struct variablet
  {
    unsigned index;
    explicit variablet(unsigned _index):index(_index) { }
    variablet() { }
  };
  
  struct row_indt
  {
    unsigned index;
    explicit row_indt(unsigned _index):index(_index) { }
    row_indt() { }
  };
  
  struct column_indt
  {
    unsigned index;
    explicit column_indt(unsigned _index):index(_index) { }
    column_indt() { }
  };
  
  friend inline bool operator==(variablet a, variablet b)
  {
    return a.index==b.index;
  }

  friend inline bool operator!=(variablet a, variablet b)
  {
    return a.index!=b.index;
  }
  
  friend inline bool operator<(column_indt a, column_indt b)
  {
    return a.index<b.index;
  }

  class rowt
  {
  public:
    // the basic variable this row corresponds to
    variablet variable;

    typedef std::map<column_indt, rationalt> coefficientst;
    coefficientst coefficients;

    rationalt get_coefficient(column_indt c) const
    {
      coefficientst::const_iterator c_it=
        coefficients.find(c);
      if(c_it==coefficients.end()) return 0;
      return c_it->second;
    }
    
    void set_coefficient(column_indt c, const rationalt &r)
    {
      coefficients[c]=r;
    }
    
    rationalt &operator[](column_indt c)
    {
      return coefficients[c];
    }
    
    void remove_zero_entries();
    void negate();
  };

  typedef std::vector<rowt> tableaut;
  tableaut tableau;

  struct columnt
  {
    // the non-basic variable this column corresponds to
    variablet variable;
  };

  typedef std::vector<columnt> columnst;
  columnst columns;

  rowt &new_row();
  
  void print_tableau(std::ostream &out) const;
  void print_tableau_latex(std::ostream &out) const;
  void print_tableau_formulas(std::ostream &out) const;

  dstring get_variable_name(variablet v) const;

  rowt &new_auxiliary_variable();
  general_simplex_tableaut();
  variablet map_variable(dstring s);
  
  inline unsigned number_of_variables() const
  {
    return variables.size();
  }
  
  inline column_indt get_column(variablet v) const
  {
    // only non-basic variables have a column
    assert(!variables[v.index].basic);
    return variables[v.index].column;
  }
  
  inline row_indt get_row(variablet v) const
  {
    // only basic variables have a row
    assert(variables[v.index].basic);
    return variables[v.index].row;
  }
  
  inline rationalt get_coefficient(
    variablet basic,
    variablet non_basic) const
  {
    return tableau[get_row(basic).index].
      get_coefficient(get_column(non_basic));
  }

  void add_lower_bound(variablet variable, rationalt value);
  void add_upper_bound(variablet variable, rationalt value);
    
  inline const rationalt &value(variablet v) const
  {
    return variables[v.index].value;
  }
  
  inline rationalt &value(variablet v)
  {
    return variables[v.index].value;
  }
  
  inline variablet variable(column_indt c) const
  {
    return columns[c.index].variable;
  }

  inline variablet variable(row_indt r) const
  {
    return tableau[r.index].variable;
  }
  
  inline const rationalt &lower_bound(variablet v) const
  {
    return variables[v.index].lower_bound.value;
  }

  inline const rationalt &upper_bound(variablet v) const
  {
    return variables[v.index].upper_bound.value;
  }

protected:
  inline variablet new_variable()
  {
    variables.resize(variables.size()+1);
    return variablet(variables.size()-1);
  }
  
  struct boundt
  {
    bool is_set;
    rationalt value;
    
    boundt():is_set(false) { }
    
    inline void set(const rationalt &v)
    {
      is_set=true;
      value=v;
    }
  };

  struct vart
  {
  public:
    dstring name;
    typedef std::set<rowt *> rowst;
    rowst rows;
    boundt upper_bound, lower_bound;
    rationalt value;

    bool basic;

    // basic variables are associated with a row,
    // non-basic variables are associated with a column
    column_indt column;
    row_indt row;
    
    inline bool bound_is_unsat() const
    {
      return upper_bound.is_set && lower_bound.is_set &&
             upper_bound.value<lower_bound.value;
    }
    
    inline bool violates_lower() const
    {
      return lower_bound.is_set && value<lower_bound.value;
    }
    
    inline bool violates_upper() const
    {
      return upper_bound.is_set && value>upper_bound.value;
    }
    
    inline bool in_bounds() const
    {
      return !violates_upper() && !violates_lower();
    }
    
    inline bool upper_bound_is_tight() const
    {
      return upper_bound.is_set && value==upper_bound.value;
    }

    inline bool lower_bound_is_tight() const
    {
      return lower_bound.is_set && value==lower_bound.value;
    }
    
    inline void make_basic(row_indt _row)
    {
      basic=true;
      row=_row;
    }
    
    inline void make_non_basic(column_indt _column)
    {
      basic=false;
      column=_column;
    }
  };
  
  typedef std::vector<vart> variablest;
  variablest variables;

  // map from dstring-indices to variables
  std::vector<variablet> variable_map;

  void compute_rows();
  
  unsigned auxiliary_variable_counter;
};

std::ostream &operator<<(std::ostream &out, const general_simplex_tableaut &s);

#endif
