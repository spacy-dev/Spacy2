#include "singularOperatorException.hh"

namespace Spacy
{
  SingularOperatorException::SingularOperatorException(const std::string& function)
    : std::runtime_error("In" + function + ": Singular operator.\n")
  {}
}
