#include "abstractOperator.hh"

#include "Interface/abstractBanachSpace.hh"

namespace Algorithm
{
  AbstractOperator::AbstractOperator(const AbstractBanachSpace &domain, const AbstractBanachSpace &range)
    : domain_(domain), range_(range)
  {}

  AbstractOperator::~AbstractOperator(){}

  const AbstractBanachSpace& AbstractOperator::getDomain() const
  {
    return domain_;
  }

  const AbstractBanachSpace& AbstractOperator::getRange() const
  {
    return range_;
  }
}
