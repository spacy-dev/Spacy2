#ifndef ALGORITHM_UTIL_CALLOFUNDEFINEDFUNCTIONEXCEPTION_HH
#define ALGORITHM_UTIL_CALLOFUNDEFINEDFUNCTIONEXCEPTION_HH

#include <stdexcept>
#include <string>

namespace Algorithm
{
  /// Exception to be thrown if a virtual function is not implemented.
  class CallOfUndefinedFunctionException : public std::runtime_error
  {
  public:
    CallOfUndefinedFunctionException(const std::string&);
  };
}
#endif // ALGORITHM_UTIL_CALLOFUNDEFINEDFUNCTIONEXCEPTION_HH
