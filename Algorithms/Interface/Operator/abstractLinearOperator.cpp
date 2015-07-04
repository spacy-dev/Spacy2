#include "abstractLinearOperator.hh"

#include "abstractOperator.hh"

namespace Algorithm
{
  Interface::AbstractLinearOperator::AbstractLinearOperator(std::shared_ptr<AbstractBanachSpace> domain, std::shared_ptr<AbstractBanachSpace> range)
    : AbstractOperator(domain,range)
  {}
}
