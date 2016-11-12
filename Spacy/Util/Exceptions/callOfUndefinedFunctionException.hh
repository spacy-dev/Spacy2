#pragma once

#include <stdexcept>
#include <string>

namespace Spacy
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
