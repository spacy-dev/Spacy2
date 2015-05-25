#include "operator.hh"

#include "Util/callofundefinedfunctionexception.hh"

namespace Algorithm
{
  Operator::Operator(const FunctionSpace& domain, const FunctionSpace& range)
    : domain_(domain), range_(range)
  {}

  const FunctionSpace& Operator::getRange() const
  {
    return range_;
  }

  const FunctionSpace& Operator::getDomain() const
  {
    return domain_;
  }

  void Operator::update(const FunctionSpaceElement& x)
  {
    throw CallOfUndefinedFunctionException("Operator::update");
  }

  FunctionSpaceElement Operator::d1(const FunctionSpaceElement&) const
  {
    throw CallOfUndefinedFunctionException("Operator::d1");
  }

}
