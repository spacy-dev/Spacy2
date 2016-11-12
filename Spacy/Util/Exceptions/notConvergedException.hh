#pragma once

#include <stdexcept>
#include <string>

namespace Spacy
{
  namespace Exception
  {
    /**
     * @ingroup ExceptionGroup
     * @brief Exception to be thrown if an algorithm did not converge.
     */
    struct NotConverged : std::runtime_error
    {
      NotConverged(const std::string& nameOfAlgorithm);
    };
  }
}
