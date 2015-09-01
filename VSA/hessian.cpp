#include "hessian.hh"

#include "boost/type_erasure/is_empty.hpp"

#include <stdexcept>
#include <utility>

namespace VSA
{
  Hessian::Hessian(C2Functional F, Vector x)
    : OperatorBase(F.domain(),F.domain().dualSpace()),
      F_(std::move(F)),
      x_(x)
  {}

  Hessian::Hessian(C2Functional F, Vector x, LinearSolver solver)
    : OperatorBase(F.domain(),F.domain().dualSpace()),
      F_(std::move(F)),
      x_(std::move(x)),
      solver_(std::move(solver))
  {}


  Vector Hessian::operator ()(const Vector& dx) const
  {
    return F_.d2(x_,dx);
  }

  const LinearSolver& Hessian::solver() const
  {
    if( is_empty(solver_) ) throw std::runtime_error("Trying to access solver that has not been set in Hessian.");
    return solver_;
  }
}
