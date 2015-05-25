#include "singularoperatorexception.hh"

namespace Algorithm
{
  SingularOperatorException::SingularOperatorException(const std::string& function)
    : std::runtime_error("In" + function + ": Singular operator.\n")
  {}
}
