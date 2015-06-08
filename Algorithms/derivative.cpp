#include "derivative.hh"

#include "c1Operator.hh"
#include "linearOperator.hh"

namespace Algorithm
{
  LinearOperator derivative(const C1Operator& A)
  {
    return LinearOperator( A.getLinearization() );
  }
}
