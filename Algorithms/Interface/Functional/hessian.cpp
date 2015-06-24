#include "hessian.hh"

#include "Interface/Functional/abstractC2Functional.hh"
#include "Interface/abstractFunctionSpaceElement.hh"

namespace Algorithm
{
  Hessian::Hessian(const AbstractC2Functional &A, const AbstractFunctionSpaceElement& x)
    : AbstractLinearOperator(A.getDomain(),A.getDomain()), A_(A), x_(x)
  {}

  Hessian::Hessian(const AbstractC2Functional &A, const AbstractFunctionSpaceElement& x, std::shared_ptr<AbstractLinearSolver> solver)
    : AbstractLinearOperator(A.getDomain(),A.getDomain()), A_(A), x_(x), solver_(solver)
  {}

  std::unique_ptr<AbstractFunctionSpaceElement> Hessian::operator ()(const AbstractFunctionSpaceElement& dx) const
  {
    return A_.getHessian(x_)(dx);
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
    return new Hessian(A_,x_,solver_);
  }
}
