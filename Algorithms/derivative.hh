#ifndef ALGORITHM_DERIVATIVE_HH
#define ALGORITHM_DERIVATIVE_HH

namespace Algorithm
{
  class C1Operator;
  class LinearOperator;

  LinearOperator derivative(const C1Operator& A);
}

#endif // ALGORITHM_DERIVATIVE_HH
