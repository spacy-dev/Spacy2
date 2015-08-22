#include "hessian.hh"

#include "vector.hh"
#include "abstractLinearSolver.hh"

#include <utility>
#include <iostream>

namespace Algorithm
{
  namespace Interface
  {
    Hessian::Hessian(Hessian&& other)
      : AbstractLinearOperator(other.domain_ptr(),other.range_ptr()),
        Mixin::UniqueImpl<AbstractFunctional>(std::move(other)),
        x_(std::move(other.x_))
    {}

    Hessian::Hessian(std::unique_ptr<AbstractFunctional>&& A, const Vector& x)
      : AbstractLinearOperator(A->domain_ptr(),A->domain_ptr()),
        Mixin::UniqueImpl<AbstractFunctional>(std::move(A)),
        x_(x)
    {}

    Vector Hessian::operator ()(const Vector& dx) const
    {
      return impl().d2(x_,dx);
    }

    std::unique_ptr<AbstractLinearSolver> Hessian::solver() const
    {
      return impl().makeSolver();
    }


    Hessian* Hessian::cloneImpl() const
    {
      return new Hessian(clone(impl()),x_);
    }
  }
}
