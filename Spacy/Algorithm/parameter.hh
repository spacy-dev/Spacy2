#ifndef SPACY_SPACY_PARAMETERS_HH
#define SPACY_SPACY_PARAMETERS_HH

#include "Util/mixins.hh"

namespace Spacy
{
  /**
   * @brief Basic parameters for simple algorithms.
   */
class Parameter : public Mixin::Eps, public Mixin::RelativeAccuracy, public Mixin::Verbosity, public Mixin::MaxSteps {};
}

#endif // SPACY_SPACY_PARAMETERS_HH
