#include "linearizedOperator.hh"

#include "abstractOperator.hh"
#include "Interface/abstractVector.hh"
#include "Interface/abstractLinearSolver.hh"

#include <utility>

namespace Algorithm
{
  namespace Interface
  {
    LinearizedOperator::LinearizedOperator(std::unique_ptr<AbstractOperator>&& A, const AbstractVector& x)
      : AbstractLinearOperator(A->domain_ptr(),A->range_ptr()),
        A_(std::move(A)), x_(clone(x))
    {}

    std::unique_ptr<AbstractVector> LinearizedOperator::operator ()(const AbstractVector& dx) const
    {
      return A_->d1(*x_,dx);
    }

    std::unique_ptr<AbstractLinearSolver> LinearizedOperator::solver() const
    {
      return A_->makeSolver();
    }

    std::unique_ptr<AbstractLinearSolver> LinearizedOperator::adjointSolver() const
    {
      return A_->makeAdjointSolver();
    }

    LinearizedOperator* LinearizedOperator::cloneImpl() const
    {
      return new LinearizedOperator(clone(A_),*x_);
    }
  }
}
