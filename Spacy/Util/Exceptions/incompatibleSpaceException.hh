#ifndef SPACY_UTIL_INCOMPATIBLESPACEEXCEPTION_HH
#define SPACY_UTIL_INCOMPATIBLESPACEEXCEPTION_HH

#include "genericException.hh"

namespace Spacy
{
  /**
   * @ingroup ExceptionGroup
   * @brief Exception to be thrown when encountering incompatible spaces.
   */
  class IncompatibleSpaceException : public GenericException
  {
  public:
    /**
     * @brief Constructor.
     * @param spaceIndex1 index of first space
     * @param spaceIndex2 index of second space
     */
    IncompatibleSpaceException(unsigned spaceIndex1, unsigned spaceIndex2);
  };
}

#endif // SPACY_UTIL_INCOMPATIBLESPACEEXCEPTION_HH
