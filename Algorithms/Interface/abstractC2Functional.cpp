#include "abstractC2Functional.hh"

#include "../Util/callofundefinedfunctionexception.hh"

namespace Algorithm
{
  AbstractC2Functional::AbstractC2Functional(const AbstractBanachSpace &domain)
    : AbstractC1Functional(domain)
  {}

  void AbstractC2Functional::getMatrix(const double *, const double *) const
  {
    throw CallOfUndefinedFunctionException("AbstractC2Functional::getMatrix");
  }
}
