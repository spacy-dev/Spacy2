#ifndef ALGORITHM_UTIL_CALL_OF_UNDEFINED_FUNCTION_EXCEPTION_HH
#define ALGORITHM_UTIL_CALL_OF_UNDEFINED_FUNCTION_EXCEPTION_HH

#include <stdexcept>
#include <string>

namespace Algorithm
{
  /**
   * @ingroup ExceptionGroup
   * @brief Exception to be thrown if a virtual function is not implemented.
   */
  class CallOfUndefinedFunctionException : public std::runtime_error
  {
  public:
    /**
     * @brief Constructor.
     * @param function name of function that throws
     */
    CallOfUndefinedFunctionException(const std::string& function);
  };
}
#endif // ALGORITHM_UTIL_CALL_OF_UNDEFINED_FUNCTION_EXCEPTION_HH
