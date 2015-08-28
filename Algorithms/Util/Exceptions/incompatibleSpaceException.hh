#ifndef ALGORITHM_UTIL_INCOMPATIBLESPACEEXCEPTION_HH
#define ALGORITHM_UTIL_INCOMPATIBLESPACEEXCEPTION_HH

#include <string>
#include <stdexcept>

namespace Algorithm
{
  /**
   * @ingroup ExceptionGroup
   * @brief Exception to be thrown when encountering incompatible spaces.
   */
  class IncompatibleSpaceException : public std::runtime_error
  {
  public:
    /**
     * @brief Constructor.
     * @param function name of function that throws
     * @param spaceIndex1 index of first space
     * @param spaceIndex2 index of second space
     */
    IncompatibleSpaceException(const std::string& function, unsigned spaceIndex1, unsigned spaceIndex2);
  };
}

#endif // ALGORITHM_UTIL_INCOMPATIBLESPACEEXCEPTION_HH
