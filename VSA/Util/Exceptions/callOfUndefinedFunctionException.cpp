#include "callOfUndefinedFunctionException.hh"

namespace VSA
{
  CallOfUndefinedFunctionException::CallOfUndefinedFunctionException(const std::string& function)
    : std::runtime_error("In " + function + ".")
  {}
}
