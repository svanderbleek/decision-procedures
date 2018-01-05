//
// Decision Procedures
// An Algorithmic Point of View
//
// Daniel Kroening, Ofer Strichman
//
// Unbounded Integers
//

#include "integer.h"

std::ostream &operator<<(std::ostream &out, const integert &n)
{
  unsigned len = n.digits(10) + 2;
  char *buffer=(char *)malloc(len);
  char *s = n.as_string(buffer, len, 10);
  out << s;
  free(buffer);
  return out;
}
