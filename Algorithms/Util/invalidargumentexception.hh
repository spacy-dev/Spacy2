#ifndef ALGORITHM_UTIL_INVALIDARGUMENTEXCEPTION_HH
#define ALGORITHM_UTIL_INVALIDARGUMENTEXCEPTION_HH

#include <string>
#include <stdexcept>

namespace Algorithm
{
  /// Exception to be thrown on invalid arguments.
  class InvalidArgumentException : public std::runtime_error
  {
  public:
    InvalidArgumentException(const std::string&);
  };
}

#endif // ALGORITHMS_UTIL_INVALIDARGUMENTEXCEPTION_HH
