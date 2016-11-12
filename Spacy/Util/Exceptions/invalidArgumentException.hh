#pragma once

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
