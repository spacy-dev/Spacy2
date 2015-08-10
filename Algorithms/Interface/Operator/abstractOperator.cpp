#include "abstractOperator.hh"

#include "Interface/abstractFunctionSpace.hh"
#include "Interface/abstractLinearSolver.hh"

namespace Algorithm
{
  namespace Interface
  {
    AbstractOperator::AbstractOperator(std::shared_ptr<AbstractFunctionSpace> domain, std::shared_ptr<AbstractFunctionSpace> range)
      : domain_(domain), range_(range)
    {}

    AbstractOperator::~AbstractOperator(){}

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
