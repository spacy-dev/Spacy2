#ifndef ALGORITHM_UTIL_REGULARITY_TEST_FAILED_EXCEPTION_HH
#define ALGORITHM_UTIL_REGULARITY_TEST_FAILED_EXCEPTION_HH

#include <stdexcept>
#include <string>

namespace VSA
{
  /**
   * @ingroup ExceptionGroup
   * @brief Exception to be thrown if regularity test fails
   */
  class RegularityTestFailedException : public std::runtime_error
  {
  public:
    /**
     * @brief Constructor.
     * @param function name of function that throws
     * @param nu damping factor
     */
    RegularityTestFailedException(const std::string& function, double nu);
  };
}

#endif // ALGORITHM_UTIL_REGULARITY_TEST_FAILED_EXCEPTION_HH
