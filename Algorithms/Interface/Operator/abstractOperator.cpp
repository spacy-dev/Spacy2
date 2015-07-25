#include "abstractOperator.hh"

#include "Interface/abstractBanachSpace.hh"
#include "Interface/abstractLinearSolver.hh"

namespace Algorithm
{
  namespace Interface
  {
    AbstractOperator::AbstractOperator(std::shared_ptr<AbstractBanachSpace> domain, std::shared_ptr<AbstractBanachSpace> range)
      : domain_(domain), range_(range)
    {}

    AbstractOperator::~AbstractOperator(){}

    AbstractBanachSpace& AbstractOperator::domain()
    {
      return *domain_;
    }

    const AbstractBanachSpace& AbstractOperator::domain() const
    {
      return *domain_;
    }

    AbstractBanachSpace& AbstractOperator::range()
    {
      return *range_;
    }

    const AbstractBanachSpace& AbstractOperator::range() const
    {
      return *range_;
    }

    std::shared_ptr<AbstractBanachSpace> AbstractOperator::sharedDomain() const
    {
      return domain_;
    }

    std::shared_ptr<AbstractBanachSpace> AbstractOperator::sharedRange() const
    {
      return range_;
    }
  }
}
