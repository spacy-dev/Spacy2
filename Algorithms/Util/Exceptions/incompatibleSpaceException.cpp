#include "incompatibleSpaceException.hh"

namespace Algorithm
{
  IncompatibleSpaceException::IncompatibleSpaceException(const std::string& function, unsigned spaceIndex1, unsigned spaceIndex2)
    : std::runtime_error("In " + function + ": incompatible space indices: " + std::to_string(spaceIndex1) + " and " + std::to_string(spaceIndex2) + ".\n")
  {}
}
