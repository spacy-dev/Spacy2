#include "linearizedFunctional.hh"

#include "abstractC2Functional.hh"
#include "Interface/abstractFunctionSpaceElement.hh"

namespace Algorithm
{
  Interface::LinearizedFunctional::LinearizedFunctional(const Interface::AbstractC2Functional &A, const Interface::AbstractFunctionSpaceElement& x)
    : Interface::AbstractFunctional(A.getSharedDomain()), A_(A), x_(x)
  {}

  double Interface::LinearizedFunctional::d0(const Interface::AbstractFunctionSpaceElement& dx) const
  {
    return A_.d1(x_,dx);
  }

  Interface::LinearizedFunctional* Interface::LinearizedFunctional::cloneImpl() const
  {
    return new Interface::LinearizedFunctional(A_,x_);
  }
}
