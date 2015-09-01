#ifndef SINGULAROPERATOREXCEPTION_HH
#define SINGULAROPERATOREXCEPTION_HH

#include <stdexcept>
#include <string>

namespace VSA
{
  /**
   * @ingroup ExceptionGroup
   * @brief Exception to be thrown if singular operators are inverted.
   */
  class SingularOperatorException : public std::runtime_error
  {
  public:
    /**
     * @brief Constructor.
     * @param function name of function that throws.
     */
    SingularOperatorException(const std::string& function);
  };
}

#endif // SINGULAROPERATOREXCEPTION_HH
