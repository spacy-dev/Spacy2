#ifndef SPACY_UTIL_EXCEPTIONS_NOT_CONVERGED_HH
#define SPACY_UTIL_EXCEPTIONS_NOT_CONVERGED_HH

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

#endif // SPACY_UTIL_EXCEPTIONS_NOT_CONVERGED_HH
