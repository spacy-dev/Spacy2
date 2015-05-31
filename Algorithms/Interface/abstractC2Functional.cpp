#include "abstractC2Functional.hh"

#include "../Util/callofundefinedfunctionexception.hh"

namespace Algorithm
{
  AbstractC2Functional::AbstractC2Functional(const AbstractBanachSpace &domain)
    : AbstractC1Functional(domain)
  {}

  std::unique_ptr<AbstractLinearSolver> AbstractC2Functional::inverse() const
  {
    throw CallOfUndefinedFunctionException("AbstractC2Functional::inverse");
  }
}
