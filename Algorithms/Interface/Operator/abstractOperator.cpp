#include "abstractOperator.hh"

#include "Interface/abstractFunctionSpace.hh"
#include "Interface/abstractLinearSolver.hh"
#include "linearizedOperator.hh"

#include "Util/Exceptions/callOfUndefinedFunctionException.hh"

namespace Algorithm
{
  namespace Interface
  {
    AbstractOperator::AbstractOperator(std::shared_ptr<AbstractFunctionSpace> domain, std::shared_ptr<AbstractFunctionSpace> range)
      : domain_(domain), range_(range)
    {}

    AbstractOperator::~AbstractOperator(){}

    std::unique_ptr<AbstractFunctionSpaceElement> AbstractOperator::d1(const AbstractFunctionSpaceElement& x,
                                                                       const AbstractFunctionSpaceElement& dx) const
    {
      throw CallOfUndefinedFunctionException("AbstractOperator::d1");
    }

    std::unique_ptr<AbstractFunctionSpaceElement> AbstractOperator::d2(const AbstractFunctionSpaceElement& x,
                                                                       const AbstractFunctionSpaceElement& dx,
                                                                       const AbstractFunctionSpaceElement& dy) const
    {
      throw CallOfUndefinedFunctionException("AbstractOperator::d2");
    }

    std::unique_ptr<LinearizedOperator> AbstractOperator::linearization(const AbstractFunctionSpaceElement& x) const
    {
      return makeLinearization(x);
    }

    std::unique_ptr<LinearizedOperator> AbstractOperator::makeLinearization(const AbstractFunctionSpaceElement& x) const
    {
      return std::make_unique<LinearizedOperator>(clone(this),x);
    }

    std::unique_ptr<AbstractLinearSolver> AbstractOperator::makeSolver() const
    {
      throw CallOfUndefinedFunctionException("AbstractOperator::makeSolver");
    }

    AbstractFunctionSpace& AbstractOperator::domain()
    {
      return *domain_;
    }

    const AbstractFunctionSpace& AbstractOperator::domain() const
    {
      return *domain_;
    }

    AbstractFunctionSpace& AbstractOperator::range()
    {
      return *range_;
    }

    const AbstractFunctionSpace& AbstractOperator::range() const
    {
      return *range_;
    }

    std::shared_ptr<AbstractFunctionSpace> AbstractOperator::sharedDomain() const
    {
      return domain_;
    }

    std::shared_ptr<AbstractFunctionSpace> AbstractOperator::sharedRange() const
    {
      return range_;
    }
  }
}
