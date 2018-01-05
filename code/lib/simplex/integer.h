//
// Decision Procedures
// An Algorithmic Point of View
//
// Daniel Kroening, Ofer Strichman
//
// Unbounded Integers
//

#ifndef INTEGER_H
#define INTEGER_H

#include <iostream>

#include "../bigint/bigint.hh"

typedef BigInt integert;

std::ostream &operator<<(std::ostream &out, const integert &i);

#endif
