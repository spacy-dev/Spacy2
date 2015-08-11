#include "abstractOperator.hh"

#include "Interface/abstractVectorSpace.hh"
#include "Interface/abstractLinearSolver.hh"
#include "linearizedOperator.hh"

#include "Util/Exceptions/callOfUndefinedFunctionException.hh"

namespace Algorithm
{
  namespace Interface
  {
    AbstractOperator::AbstractOperator(std::shared_ptr<AbstractVectorSpace> domain, std::shared_ptr<AbstractVectorSpace> range)
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

    AbstractVectorSpace& AbstractOperator::domain()
    {
      return *domain_;
    }

    const AbstractVectorSpace& AbstractOperator::domain() const
    {
      return *domain_;
    }

    AbstractVectorSpace& AbstractOperator::range()
    {
      return *range_;
    }

    const AbstractVectorSpace& AbstractOperator::range() const
    {
      return *range_;
    }

    std::shared_ptr<AbstractVectorSpace> AbstractOperator::sharedDomain() const
    {
      return domain_;
    }

    std::shared_ptr<AbstractVectorSpace> AbstractOperator::sharedRange() const
    {
      return range_;
    }
  }
}
