#ifndef ALGORITHM_GENERIC_EXCEPTION_HH
#define ALGORITHM_GENERIC_EXCEPTION_HH

#include <string>
#include <stdexcept>

namespace Algorithm
{
  /**
   * @ingroup ExceptionGroup
   * @brief A generic exception class that serves as base for all exceptions in this library.
   */
  class GenericException : public std::runtime_error
  {
  public:
    /**
     * @brief Constructor.
     * @param exception name of the exception
     * @param message exception message
     * @param hint hint for removing the reason for the exception
     */
    GenericException(const std::string& exception,
                     const std::string& message,
                     const std::string& hint = {});
  };
}

#endif // ALGORITHM_GENERIC_EXCEPTION_HH
