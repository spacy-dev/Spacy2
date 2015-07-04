#include "hessian.hh"

#include "Interface/abstractFunctionSpaceElement.hh"

#include <utility>

namespace Algorithm
{
  namespace Interface
  {
    Hessian::Hessian(std::unique_ptr<AbstractC2Functional>&& A, const AbstractFunctionSpaceElement& x)
      : AbstractLinearOperator(A->getSharedDomain(),A->getSharedDomain()), Mixin::UniqueImpl<AbstractC2Functional>(std::move(A)), x_(x)
    {}

    Hessian::Hessian(std::unique_ptr<AbstractC2Functional>&& A, const AbstractFunctionSpaceElement& x, std::shared_ptr<AbstractLinearSolver> solver)
      : AbstractLinearOperator(A->getSharedDomain(),A->getSharedDomain()), Mixin::UniqueImpl<AbstractC2Functional>(std::move(A)), x_(x), solver_(solver)
    {}

    std::unique_ptr<AbstractFunctionSpaceElement> Hessian::operator ()(const AbstractFunctionSpaceElement& dx) const
    {
      return impl().d2(x_,dx);
    }

    std::shared_ptr<AbstractLinearSolver> Hessian::getSolver() const
    {
      return solver_;
    }

    void Hessian::setSolver(std::shared_ptr<AbstractLinearSolver> solver)
    {
      solver_ = solver;
    }

    Hessian* Hessian::cloneImpl() const
    {
      return new Hessian(clone(impl()),x_,solver_);
    }
  }
}
