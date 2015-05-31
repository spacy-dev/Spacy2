#ifndef ALGORITHM_DERIVATIVE_HH
#define ALGORITHM_DERIVATIVE_HH

#include "c0Functional.hh"
#include "c1Functional.hh"

#include "c0Operator.hh"
#include "c1Operator.hh"

namespace Algorithm
{
  class C2Functional;
  class C2Operator;

  C0Operator derivative(const C1Operator& A);

  C1Operator derivative(const C2Operator& A);

  C0Functional derivative(const C1Functional& A);

  C1Functional derivative(const C2Functional& A);
}

#endif // ALGORITHM_DERIVATIVE_HH
