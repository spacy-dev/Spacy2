#include "abstractLinearOperator.hh"

#include "abstractOperator.hh"

namespace Algorithm
{
  AbstractLinearOperator::AbstractLinearOperator(const AbstractBanachSpace &domain, const AbstractBanachSpace &range)
    : AbstractOperator(domain,range)
  {}
}
