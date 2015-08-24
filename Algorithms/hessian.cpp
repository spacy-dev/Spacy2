#include "hessian.hh"

#include <utility>

namespace Algorithm
{
  Hessian::Hessian(C2Functional F, const Vector& x)
    : OperatorBase(F.domain_ptr(),F.domain_ptr()),
      F_(std::move(F)),
      x_(x)
  {}

  Hessian::Hessian(C2Functional F, const Vector &x, std::shared_ptr<LinearSolver> solver)
    : Hessian(F,x)
  {
    solver_ = solver;
  }


  Vector Hessian::operator ()(const Vector& dx) const
  {
    return F_.d2(x_,dx);
  }

  const LinearSolver& Hessian::solver() const
  {
    return *solver_;
  }
}
