#ifndef ALGORITHM_UTIL_CALLOFUNDEFINEDFUNCTIONEXCEPTION_HH
#define ALGORITHM_UTIL_CALLOFUNDEFINEDFUNCTIONEXCEPTION_HH

#include <stdexcept>
#include <string>

namespace Algorithm
{
  class CallOfUndefinedFunctionException : public std::runtime_error
  {
  public:
    CallOfUndefinedFunctionException(const std::string&);
  };
}
#endif // ALGORITHM_UTIL_CALLOFUNDEFINEDFUNCTIONEXCEPTION_HH
