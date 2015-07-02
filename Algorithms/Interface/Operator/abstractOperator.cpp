#include "abstractOperator.hh"

#include "Interface/abstractBanachSpace.hh"

namespace Algorithm
{
  AbstractOperator::AbstractOperator(std::shared_ptr<AbstractBanachSpace> domain, std::shared_ptr<AbstractBanachSpace> range)
    : domain_(domain), range_(range)
  {}

  AbstractOperator::~AbstractOperator(){}

  AbstractBanachSpace& AbstractOperator::getDomain()
  {
    return *domain_;
  }

  const AbstractBanachSpace& AbstractOperator::getDomain() const
  {
    return *domain_;
  }

  AbstractBanachSpace& AbstractOperator::getRange()
  {
    return *range_;
  }

  const AbstractBanachSpace& AbstractOperator::getRange() const
  {
    return *range_;
  }

  std::shared_ptr<AbstractBanachSpace> AbstractOperator::getSharedDomain() const
  {
    return domain_;
  }

  std::shared_ptr<AbstractBanachSpace> AbstractOperator::getSharedRange() const
  {
    return range_;
  }
}
