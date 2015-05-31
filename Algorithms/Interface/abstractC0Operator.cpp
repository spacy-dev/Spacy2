#include "abstractC0Operator.hh"

#include "abstractFunctionSpaceElement.hh"

#include "../Util/callofundefinedfunctionexception.hh"

namespace Algorithm
{
  AbstractC0Operator::AbstractC0Operator(const AbstractBanachSpace &domain, const AbstractBanachSpace &range)
    : domain_(domain), range_(range)
  {}

  AbstractC0Operator::~AbstractC0Operator(){}

//  std::unique_ptr<AbstractFunctionSpaceElement> AbstractC0Operator::operator()(const AbstractFunctionSpaceElement& x)
//  {
//    setArgument(x);
//    return d0();
//  }

  const AbstractBanachSpace& AbstractC0Operator::getDomain() const
  {
    return domain_;
  }

  const AbstractBanachSpace& AbstractC0Operator::getRange() const
  {
    return range_;
  }

  std::unique_ptr<AbstractLinearSolver> AbstractC0Operator::inverse()  const
  {
    throw CallOfUndefinedFunctionException("AbstractC0Operawtor::inverse");
  }
}
