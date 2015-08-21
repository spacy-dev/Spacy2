#include "abstractOperator.hh"

#include "vectorSpace.hh"
#include "Interface/abstractLinearSolver.hh"
#include "linearizedOperator.hh"

#include "Util/Exceptions/callOfUndefinedFunctionException.hh"

namespace Algorithm
{
  namespace Interface
  {
    AbstractOperator::AbstractOperator(VectorSpace* domain, VectorSpace* range)
      : domain_(domain), range_(range)
    {}

    AbstractOperator::~AbstractOperator(){}

    std::unique_ptr<AbstractVector> AbstractOperator::d1(const AbstractVector& x,
                                                                       const AbstractVector& dx) const
    {
      throw CallOfUndefinedFunctionException("AbstractOperator::d1");
    }

    std::unique_ptr<AbstractVector> AbstractOperator::d2(const AbstractVector& x,
                                                                       const AbstractVector& dx,
                                                                       const AbstractVector& dy) const
    {
      throw CallOfUndefinedFunctionException("AbstractOperator::d2");
    }

    std::unique_ptr<LinearizedOperator> AbstractOperator::linearization(const AbstractVector& x) const
    {
      return makeLinearization(x);
    }

    std::unique_ptr<LinearizedOperator> AbstractOperator::makeLinearization(const AbstractVector& x) const
    {
      return std::make_unique<LinearizedOperator>(clone(this),x);
    }

    std::unique_ptr<AbstractLinearSolver> AbstractOperator::makeSolver() const
    {
      throw CallOfUndefinedFunctionException("AbstractOperator::makeSolver");
    }

    std::unique_ptr<AbstractLinearSolver> AbstractOperator::makeAdjointSolver() const
    {
      throw CallOfUndefinedFunctionException("AbstractOperator::makeAdjointSolver");
    }

    VectorSpace& AbstractOperator::domain()
    {
      return *domain_;
    }

    const VectorSpace& AbstractOperator::domain() const
    {
      return *domain_;
    }

    VectorSpace& AbstractOperator::range()
    {
      return *range_;
    }

    const VectorSpace& AbstractOperator::range() const
    {
      return *range_;
    }

    VectorSpace* AbstractOperator::domain_ptr() const
    {
      return domain_;
    }

    VectorSpace*  AbstractOperator::range_ptr() const
    {
      return range_;
    }
  }
}
