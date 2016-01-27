#ifndef SPACY_UTIL_INVALID_ARGUMENT_EXCEPTION_HH
#define SPACY_UTIL_INVALID_ARGUMENT_EXCEPTION_HH

#include <string>
#include <stdexcept>

namespace Spacy
{
  namespace Exception
  {
    /**
     * @ingroup ExceptionGroup
     * @brief Exception to be thrown on invalid arguments.
     */
    class InvalidArgument : public std::runtime_error
    {
    public:
      /**
       * @brief Constructor.
       * @param function name of function that throws
       */
      InvalidArgument(const std::string& function);
    };
  }
}

#endif // SPACY_UTIL_INVALID_ARGUMENT_EXCEPTION_HH
