#include "invalidArgumentException.hh"

namespace VSA
{
  InvalidArgumentException::InvalidArgumentException(const std::string& function)
    : std::runtime_error("In " + function + ": Invalid arguments.\n")
  {}
}
