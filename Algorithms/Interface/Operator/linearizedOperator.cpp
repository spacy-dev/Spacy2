#include "linearizedOperator.hh"

#include "abstractC1Operator.hh"
#include "Interface/Functional/abstractC2Functional.hh"
#include "Interface/abstractFunctionSpaceElement.hh"

#include <stdexcept>
#include <utility>

namespace Algorithm
{
  namespace Interface
  {
    LinearizedOperator::LinearizedOperator(std::unique_ptr<AbstractC1Operator>&& A, const AbstractFunctionSpaceElement& x)
      : AbstractLinearOperator(A->getSharedDomain(),A->getSharedRange()), A_(std::move(A)), x_(x)
    {}

    LinearizedOperator::LinearizedOperator(std::unique_ptr<AbstractC1Operator>&& A, const AbstractFunctionSpaceElement& x, std::shared_ptr<AbstractLinearSolver> solver)
      : AbstractLinearOperator(A->getSharedDomain(),A->getSharedRange()), A_(std::move(A)), x_(x), solver_(solver)
    {}

    std::unique_ptr<AbstractFunctionSpaceElement> LinearizedOperator::operator ()(const AbstractFunctionSpaceElement& dx) const
    {
      return A_->d1(x_,dx);
    }

    std::shared_ptr<AbstractLinearSolver> LinearizedOperator::getSolver() const
    {
      if( solver_ == nullptr ) throw std::runtime_error("No solver defined in LinearizedOperator.");
      return solver_;
    }

    void LinearizedOperator::setSolver(std::shared_ptr<AbstractLinearSolver> solver)
    {
      solver_ = solver;
    }

    LinearizedOperator* LinearizedOperator::cloneImpl() const
    {
      return new LinearizedOperator(clone(A_),x_,solver_);
    }
  }
}
