/*******************************************************************\

Module:

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#include <assert.h>

#include <stack>

#include "satcheck_minisat.h"

#include <Solver.h>
#include <Proof.h>

class minisat_coret:public ProofTraverser
{
public:
  virtual void root(const vec<Lit> &c)
  {
    clauses.push_back(clauset());
    clauses.back().root=true;
    clauses.back().ids.resize(c.size());

    for(int i=0; i<c.size(); i++)
      clauses.back().ids[i]=var(c[i]);
  }
  
  virtual void chain(const vec<ClauseId> &cs, const vec<Var> &xs)
  {
    clauses.push_back(clauset());
    clauses.back().root=false;
    clauses.back().ids.resize(cs.size());

    // copy clause IDs
    int c_id=clauses.size();
    for(int i=0; i<cs.size(); i++)
    {
      // must be decreasing
      assert(cs[i]<c_id);
      clauses.back().ids[i]=cs[i];
    }
  }

  virtual void deleted(ClauseId c) { }
  virtual void done() { }
  virtual ~minisat_coret() { }
  
  struct clauset
  {
    // these may contain
    // a) literals (if it's a problem/root clause)
    // b) clause ids (if it's a derived clause)
    std::vector<int> ids;
    bool root;
  };
  
  typedef std::vector<clauset> clausest;
  clausest clauses;
  
  void build_core(std::vector<bool> &in_core);
};

/*******************************************************************\

Function: minisat_coret::build_core

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void minisat_coret::build_core(std::vector<bool> &in_core)
{
  std::stack<unsigned> s;
  std::vector<bool> seen;

  assert(!clauses.empty());
  
  seen.resize(clauses.size(), false);
  
  s.push(clauses.size()-1);
  
  while(!s.empty())
  {
    int c_id=s.top();
    s.pop();

    if(seen[c_id]) continue;
    seen[c_id]=true;
    
    const clauset &c=clauses[c_id];
    
    if(c.root)
    {
      for(unsigned i=0; i<c.ids.size(); i++)
      {
        unsigned v=c.ids[i];
        assert(v<in_core.size());
        in_core[v]=true;
      }
    }
    else
    {
      for(unsigned i=0; i<c.ids.size(); i++)
      {
        // must decrease
        assert(c.ids[i]<c_id);
        s.push(c.ids[i]);
      }
    }
  }
}

/*******************************************************************\

Function: satcheck_minisat_baset::l_get

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

tvt satcheck_minisat_baset::l_get(literalt a) const
{
  assert(status==SAT);

  if(a.is_true())
    return tvt(true);
  else if(a.is_false())
    return tvt(false);

  tvt result;

  assert(a.var_no()<(unsigned)solver->model.size());

  if(solver->model[a.var_no()]==l_True)
    result=tvt(true);
  else if(solver->model[a.var_no()]==l_False)
    result=tvt(false);
  else
    result=tvt(tvt::TV_UNKNOWN);
  
  if(a.sign()) result=!result;

  return result;
}

/*******************************************************************\

Function: satcheck_minisat_baset::solver_text

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

const std::string satcheck_minisat_baset::solver_text()
{
  return "MiniSAT";
}

/*******************************************************************\

Function: satcheck_minisat_baset::add_variables

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void satcheck_minisat_baset::add_variables()
{
  while((unsigned)solver->nVars()<(no_variables()+1))
    solver->newVar();
}

/*******************************************************************\

Function: satcheck_minisat_baset::lcnf

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void satcheck_minisat_baset::lcnf(const bvt &bv)
{
  bvt new_bv;
  
  if(process_clause(bv, new_bv))
    return;

  add_variables();

  vec<Lit> c;
  
  c.growTo(new_bv.size());
  
  for(unsigned i=0; i<new_bv.size(); i++)
  {
    c[i]=Lit(new_bv[i].var_no(), new_bv[i].sign());
    assert(new_bv[i].var_no()<(unsigned)solver->nVars());
  }

  solver->addClause(c);
  
  clause_counter++;
}

/*******************************************************************\

Function: satcheck_minisat_baset::prop_solve

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

propt::resultt satcheck_minisat_baset::prop_solve()
{
  assert(status!=ERROR);

  add_variables();
  
  if(solver->okay())
    solver->solve();

  if(solver->okay())
  {
    status=SAT;
    return P_SATISFIABLE;
  }
  
  status=UNSAT;

  return P_UNSATISFIABLE;
}

/*******************************************************************\

Function: satcheck_minisat_baset::set_assignment

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void satcheck_minisat_baset::set_assignment(literalt a, bool value)
{
  unsigned v=a.var_no();
  bool sign=a.sign();
  assert(v<(unsigned)solver->model.size());
  value^=sign;
  solver->model[v]=lbool(value);
}

/*******************************************************************\

Function: satcheck_minisatt::satcheck_minisatt

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

satcheck_minisatt::satcheck_minisatt()
{
  solver=new Solver;
}

/*******************************************************************\

Function: satcheck_minisat_coret::satcheck_minisat_coret

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

satcheck_minisat_coret::satcheck_minisat_coret()
{
  minisat_core=new minisat_coret;
  proof=new Proof(*minisat_core);
  solver=new Solver;
  solver->proof=proof;
}

/*******************************************************************\

Function: satcheck_minisat_coret::~satcheck_minisat_coret

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

satcheck_minisat_coret::~satcheck_minisat_coret()
{
  delete proof;
  delete minisat_core;
}

/*******************************************************************\

Function: satcheck_minisat_baset::~satcheck_minisat_baset

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

satcheck_minisat_baset::~satcheck_minisat_baset()
{
  delete solver;
}

/*******************************************************************\

Function: satcheck_minisat_coret::prop_solve

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

propt::resultt satcheck_minisat_coret::prop_solve()
{
  assert(status!=ERROR);

  {
    std::cout << _no_variables << " variables, "
              << solver->nClauses() << " clauses"
              << std::endl;
  }

  add_variables();
  
  if(solver->okay())
    solver->solve();

  {
    if(solver->okay())
      std::cout << "SAT checker: negated claim is SATISFIABLE, i.e., does not hold";
    else
      std::cout << "SAT checker: negated claim is UNSATISFIABLE, i.e., holds";

    std::cout << std::endl;
  }

  if(solver->okay())
  {
    status=SAT;
    return P_SATISFIABLE;
  }
  
  status=UNSAT;

  in_core.resize(no_variables()+1, false);
  minisat_core->build_core(in_core);
  
  return P_UNSATISFIABLE;
}

/*******************************************************************\

Function: satcheck_minisat_coret::solver_text

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

const std::string satcheck_minisat_coret::solver_text()
{
  return "MiniSAT + Core";
}

