#include "hessian.hh"

#include "abstractFunctionSpaceElement.hh"
#include "abstractLinearSolver.hh"

#include <utility>

namespace Algorithm
{
  namespace Interface
  {
//    Hessian::Hessian(std::unique_ptr<AbstractFunctional>&& A, const AbstractFunctionSpaceElement& x, std::shared_ptr<AbstractLinearSolver> solver)
//      : AbstractLinearOperator(A->getSharedDomain(),A->getSharedDomain()), Mixin::UniqueImpl<AbstractFunctional>(std::move(A)), x_(clone(x)), solver_(solver)
//    {}

    Hessian::Hessian(Hessian&& other)
      : AbstractLinearOperator(other.sharedDomain(),other.sharedRange()),
        Mixin::UniqueImpl<AbstractFunctional>(std::move(other)),
        x_(std::move(other.x_))
    {
      std::cout << "moving hessian" << std::endl;
    }

    Hessian::Hessian(std::unique_ptr<AbstractFunctional>&& A, const AbstractFunctionSpaceElement& x)
      : AbstractLinearOperator(A->sharedDomain(),A->sharedDomain()),
        Mixin::UniqueImpl<AbstractFunctional>(std::move(A)),
        x_(clone(x))
    {}

    std::unique_ptr<AbstractFunctionSpaceElement> Hessian::operator ()(const AbstractFunctionSpaceElement& dx) const
    {
      return impl().d2(*x_,dx);
    }

    std::unique_ptr<AbstractLinearSolver> Hessian::solver() const
    {
//      if( solver_ == nullptr ) throw std::runtime_error("No solver defined in Hessian.");
      return impl().makeSolver();
    }


    Hessian* Hessian::cloneImpl() const
    {
      std::cout << "cloning hessian" << std::endl;
      return new Hessian(clone(impl()),*x_);
    }
  }
}
