/*******************************************************************\

Module:

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#ifndef CPROVER_SATCHECK_MINISAT_H
#define CPROVER_SATCHECK_MINISAT_H

#include <assert.h>

#include <vector>
#include <set>

#include "cnf.h"

class satcheck_minisat_baset:public cnf_solvert
{
public:
  satcheck_minisat_baset():solver(NULL)
  {
  }
  
  virtual ~satcheck_minisat_baset();
  
  virtual const std::string solver_text();
  virtual resultt prop_solve();
  virtual tvt l_get(literalt a) const;

  virtual void lcnf(const bvt &bv);
  
  virtual void set_assignment(literalt a, bool value);

protected:
  class Solver *solver;
  void add_variables();
};

class satcheck_minisatt:public satcheck_minisat_baset
{
public:
  satcheck_minisatt();
};

class satcheck_minisat_coret:public satcheck_minisat_baset
{
public:
  satcheck_minisat_coret();
  ~satcheck_minisat_coret();
  
  virtual const std::string solver_text();
  virtual resultt prop_solve();
  
  bool is_in_core(literalt l) const
  {
    assert(l.var_no()<in_core.size());
    return in_core[l.var_no()];
  }
  
protected:
  std::vector<bool> in_core;
  class Proof *proof;
  class minisat_coret *minisat_core;
};

#endif
