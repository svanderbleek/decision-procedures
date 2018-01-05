#include <assert.h>

#include "lazy_base.h"

bool lazy_baset::get(const ast::nodet _f)
{
  // look up the node
  const skeletont::nodet &n=skeleton.nodes[_f.id()];
  assert(n.type==skeletont::nodet::PROPOSITIONAL);

  // get its value from the SAT solver
  return get_solver().l_get(n.l).is_true();
}
