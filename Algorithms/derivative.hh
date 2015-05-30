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

  C0Operator derivative(const C1Operator& A, const FunctionSpaceElement& x);

  C1Operator derivative(const C2Operator& A, const FunctionSpaceElement& x);

  C0Functional derivative(const C1Functional& A, const FunctionSpaceElement& x);

  C1Functional derivative(const C2Functional& A, const FunctionSpaceElement& x);
}

#endif // ALGORITHM_DERIVATIVE_HH
