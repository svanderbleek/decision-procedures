/*******************************************************************\

Module: CNF Generation

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#ifndef CPROVER_PROP_CNF_CLAUSE_LIST_H
#define CPROVER_PROP_CNF_CLAUSE_LIST_H

#include "cnf.h"

class cnf_clause_listt:public cnft
{
public:
  cnf_clause_listt() { }
  virtual ~cnf_clause_listt() { }
 
  virtual void lcnf(const bvt &bv);

  virtual const std::string solver_text()
  { return "CNF clause list"; }
   
  virtual tvt l_get(literalt literal) const
  {
    return tvt(tvt::TV_UNKNOWN);
  }

  virtual resultt prop_solve() { return P_ERROR; }
  
  virtual unsigned no_clauses() const { return clauses.size(); }
  
  typedef std::list<bvt> clausest;
  
  clausest &get_clauses() { return clauses; }
  
  void copy_to(cnft &cnf) const
  {
    cnf.set_no_variables(_no_variables);
    for(clausest::const_iterator
        it=clauses.begin();
        it!=clauses.end();
        it++)
      cnf.lcnf(*it);
  }
  
  static size_t hash_clause(const bvt &bv)
  {
    size_t result=0;
    for(bvt::const_iterator it=bv.begin(); it!=bv.end(); it++)
      result=(result<<2)^it->get()-result;

    return result;
  }
  
  size_t hash() const
  {
    size_t result=0;
    for(clausest::const_iterator it=clauses.begin(); it!=clauses.end(); it++)
      result=(result<<2)^hash_clause(*it)-result;

    return result;
  }
  
protected:
  clausest clauses;
};

class cnf_clause_list_assignmentt:public cnf_clause_listt
{
public:
  typedef std::vector<tvt> assignmentt;
  
  assignmentt &get_assignment()
  {
    return assignment;
  }
  
  virtual tvt l_get(literalt literal) const
  {
    unsigned v=literal.var_no();

    if(v==0 || v>=assignment.size())
      return tvt(tvt::TV_UNKNOWN);

    tvt r=assignment[v];
    return literal.sign()?!r:r;
  }
  
  virtual void copy_assignment_from(const propt &prop);
  void print_assignment(std::ostream &out) const;

protected:
  assignmentt assignment;
};

#endif
