#ifndef SPACY_NEWTON_PARAMETER_HH
#define SPACY_NEWTON_PARAMETER_HH

#include "Spacy/Util/mixins.hh"

namespace Spacy
{
  namespace Newton
  {
    struct Parameter :
        public Mixin::Eps, public Mixin::RelativeAccuracy, public Mixin::Verbosity,
        public Mixin::MaxSteps , public Mixin::RegularityTest , public Mixin::Timer<std::chrono::seconds>
    {};
  }
}

#endif // SPACY_NEWTON_PARAMETER_HH
