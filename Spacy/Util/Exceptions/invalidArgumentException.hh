#ifndef SPACY_UTIL_INVALIDARGUMENTEXCEPTION_HH
#define SPACY_UTIL_INVALIDARGUMENTEXCEPTION_HH

#include <string>
#include <stdexcept>

namespace Spacy
{
  /**
   * @ingroup ExceptionGroup
   * @brief Exception to be thrown on invalid arguments.
   */
  class InvalidArgumentException : public std::runtime_error
  {
  public:
    /**
     * @brief Constructor.
     * @param function name of function that throws
     */
    InvalidArgumentException(const std::string& function);
  };
}

#endif // SPACYS_UTIL_INVALIDARGUMENTEXCEPTION_HH
