//
// Decision Procedures
// An Algorithmic Point of View
//
// Daniel Kroening, Ofer Strichman
//
// Rational Numbers
//

#ifndef RATIONAL_H
#define RATIONAL_H

#include <assert.h>

#include <vector>

#include "integer.h"

class rationalt
{
 protected:
  integert numerator; // Zaehler
  integert denominator; // Nenner

  void normalize();
  void same_denominator(rationalt &n);

 public:
  // constructors
  rationalt():numerator(0), denominator(1) { }
  rationalt(const integert &i):numerator(i), denominator(1) { }
  rationalt(int i):numerator(i), denominator(1) { }
  
  integert get_numerator() const
  {
    return numerator;
  }
  
  integert get_denominator() const
  {
    return denominator;
  }
  
  rationalt &operator+=(const rationalt &n)
  {
    rationalt tmp(n);
    same_denominator(tmp);
    numerator+=tmp.numerator;
    normalize();
    return *this;
  }

  rationalt &operator-=(const rationalt &n)
  {
    rationalt tmp(n);
    same_denominator(tmp);
    numerator-=tmp.numerator;
    normalize();
    return *this;
  }

  rationalt &operator*=(const rationalt &n)
  {
    numerator*=n.numerator;
    denominator*=n.denominator;
    normalize();
    return *this;
  }

  rationalt &operator/=(const rationalt &n)
  {
    assert(!n.numerator.is_zero());
    numerator*=n.denominator;
    denominator*=n.numerator;
    normalize();
    return *this;
  }

  bool operator==(const rationalt &n) const
  {
    rationalt r1(*this), r2(n);
    r1.same_denominator(r2);
    return r1.numerator==r2.numerator;
  }
   
  bool operator!=(const rationalt &n) const
  {
    rationalt r1(*this), r2(n);
    r1.same_denominator(r2);
    return r1.numerator!=r2.numerator;
  }
   
  bool operator<(const rationalt &n) const
  {
    rationalt r1(*this), r2(n);
    r1.same_denominator(r2);
    return r1.numerator<r2.numerator;
  }
   
  bool operator<=(const rationalt &n) const
  {
    rationalt r1(*this), r2(n);
    r1.same_denominator(r2);
    return r1.numerator<=r2.numerator;
  }
   
  bool operator>=(const rationalt &n) const
  {
    return !(*this<n);
  }
   
  bool operator>(const rationalt &n) const
  {
    return !(*this<=n);
  }
   
  bool is_zero() const
  { return numerator.is_zero(); }
   
  bool is_negative() const
  { return !is_zero() && numerator.is_negative(); }

  void invert();
  void negate() { numerator.negate(); }

  friend rationalt operator+(const rationalt &a, const rationalt &b)
  {
    rationalt tmp(a);
    tmp+=b;
    return tmp;
  }

  friend rationalt operator-(const rationalt &a, const rationalt &b)
  {
    rationalt tmp(a);
    tmp-=b;
    return tmp;
  }

  friend rationalt operator-(const rationalt &a)
  {
    rationalt tmp(a);
    tmp.negate();
    return tmp;
  }

  friend rationalt operator*(const rationalt &a, const rationalt &b)
  {
    rationalt tmp(a);
    tmp*=b;
    return tmp;
  }

  friend rationalt operator/(const rationalt &a, const rationalt &b)
  {
    rationalt tmp(a);
    tmp/=b;
    return tmp;
  }

  friend std::ostream& operator<< (std::ostream& out, const rationalt &a);
};
 
rationalt inverse(const rationalt &n);
rationalt rational_from_string(const std::string &s);

#endif
