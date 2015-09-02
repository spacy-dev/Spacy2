#include "callOfUndefinedFunctionException.hh"

namespace Spacy
{
  CallOfUndefinedFunctionException::CallOfUndefinedFunctionException(const std::string& function)
    : std::runtime_error("In " + function + ".")
  {}
}
