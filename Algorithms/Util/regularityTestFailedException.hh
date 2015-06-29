#ifndef ALGORITHM_UTIL_REGULARITY_TEST_FAILED_EXCEPTION_HH
#define ALGORITHM_UTIL_REGULARITY_TEST_FAILED_EXCEPTION_HH

#include <stdexcept>
#include <string>

namespace Algorithm
{
  /// Exception to be thrown if a virtual function is not implemented.
  class RegularityTestFailedException : public std::runtime_error
  {
  public:
    RegularityTestFailedException(const std::string&, double nu);
  };
}

#endif // ALGORITHM_UTIL_REGULARITY_TEST_FAILED_EXCEPTION_HH
