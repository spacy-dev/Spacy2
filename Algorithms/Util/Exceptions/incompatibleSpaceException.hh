#ifndef ALGORITHM_UTIL_INCOMPATIBLESPACEEXCEPTION_HH
#define ALGORITHM_UTIL_INCOMPATIBLESPACEEXCEPTION_HH

#include <string>
#include <stdexcept>

namespace Algorithm
{
  /// Exception to be thrown when encountering incompatible spaces.
  class IncompatibleSpaceException : public std::runtime_error
  {
  public:
    IncompatibleSpaceException(const std::string& function, unsigned spaceIndex1, unsigned spaceIndex2);
  };
}

#endif // ALGORITHM_UTIL_INCOMPATIBLESPACEEXCEPTION_HH
