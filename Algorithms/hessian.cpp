#include "hessian.hh"

#include <utility>

namespace Algorithm
{
  Hessian::Hessian(C2Functional F, const Vector& x)
    : OperatorBase(F.domain_ptr(),F.domain_ptr()),
      F_(std::move(F)),
      x_(x)
  {}

  Vector Hessian::operator ()(const Vector& dx) const
  {
    return F_.d2(x_,dx);
  }

  LinearSolver Hessian::solver() const
  {
    return F_.solver();
  }
}
