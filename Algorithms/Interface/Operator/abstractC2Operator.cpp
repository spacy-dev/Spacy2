#include "abstractC2Operator.hh"

#include "Interface/abstractFunctionSpaceElement.hh"

namespace Algorithm
{
  namespace Interface
  {
    AbstractC2Operator::AbstractC2Operator(std::shared_ptr<AbstractBanachSpace> domain, std::shared_ptr<AbstractBanachSpace> range)
      : AbstractC1Operator(domain,range)
    {}
  }
}
