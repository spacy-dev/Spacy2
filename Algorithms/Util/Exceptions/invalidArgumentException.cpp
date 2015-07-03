#include "invalidArgumentException.hh"

namespace Algorithm
{
  InvalidArgumentException::InvalidArgumentException(const std::string& function)
    : std::runtime_error("In " + function + ": Invalid arguments.\n")
  {}
}
