#include "linearizedFunctional.hh"

#include "abstractC2Functional.hh"
#include "Interface/abstractFunctionSpaceElement.hh"

namespace Algorithm
{
  LinearizedFunctional::LinearizedFunctional(const AbstractC2Functional &A, const AbstractFunctionSpaceElement& x)
    : AbstractFunctional(A.getSharedDomain()), A_(A), x_(x)
  {}

  double LinearizedFunctional::d0(const AbstractFunctionSpaceElement& dx) const
  {
    return A_.d1(x_,dx);
  }

  LinearizedFunctional* LinearizedFunctional::cloneImpl() const
  {
    return new LinearizedFunctional(A_,x_);
  }
}
