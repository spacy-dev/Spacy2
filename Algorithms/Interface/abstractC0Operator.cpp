#include "abstractC0Operator.hh"

namespace Algorithm
{
  AbstractC0Operator::AbstractC0Operator(const AbstractBanachSpace &domain, const AbstractBanachSpace &range)
    : domain_(domain), range_(range)
  {}

  AbstractC0Operator::~AbstractC0Operator(){}

  const AbstractBanachSpace& AbstractC0Operator::getDomain() const
  {
    return domain_;
  }

  const AbstractBanachSpace& AbstractC0Operator::getRange() const
  {
    return range_;
  }
}
