#include "regularityTestFailedException.hh"

namespace Algorithm
{
  RegularityTestFailedException::RegularityTestFailedException(const std::string& function, double nu)
    : std::runtime_error("Regularity test failed in " + function + "(val = " + std::to_string(nu) + ")")
  {}
}

