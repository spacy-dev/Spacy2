#include "hessian.hh"

#include "abstractVector.hh"
#include "abstractLinearSolver.hh"

#include <utility>
#include <iostream>

namespace Algorithm
{
  namespace Interface
  {
    Hessian::Hessian(Hessian&& other)
      : AbstractLinearOperator(other.sharedDomain(),other.sharedRange()),
        Mixin::UniqueImpl<AbstractFunctional>(std::move(other)),
        x_(std::move(other.x_))
    {}

    Hessian::Hessian(std::unique_ptr<AbstractFunctional>&& A, const AbstractVector& x)
      : AbstractLinearOperator(A->sharedDomain(),A->sharedDomain()),
        Mixin::UniqueImpl<AbstractFunctional>(std::move(A)),
        x_(clone(x))
    {}

    std::unique_ptr<AbstractVector> Hessian::operator ()(const AbstractVector& dx) const
    {
      return impl().d2(*x_,dx);
    }

    std::unique_ptr<AbstractLinearSolver> Hessian::solver() const
    {
      return impl().makeSolver();
    }


    Hessian* Hessian::cloneImpl() const
    {
      return new Hessian(clone(impl()),*x_);
    }
  }
}
