#pragma once

#include <stdexcept>
#include <string>

namespace Spacy
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
