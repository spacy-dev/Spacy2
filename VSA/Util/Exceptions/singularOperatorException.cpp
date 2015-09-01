#include "singularOperatorException.hh"

namespace VSA
{
  SingularOperatorException::SingularOperatorException(const std::string& function)
    : std::runtime_error("In" + function + ": Singular operator.\n")
  {}
}
