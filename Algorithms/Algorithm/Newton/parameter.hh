#ifndef ALGORITHM_NEWTON_PARAMETER_HH
#define ALGORITHM_NEWTON_PARAMETER_HH

#include "Util/mixins.hh"

namespace Algorithm
{
  namespace Newton
  {
    struct Parameter :
        public Mixin::Eps, public Mixin::RelativeAccuracy, public Mixin::Verbosity,
        public Mixin::MaxSteps , public Mixin::RegularityTest , public Mixin::Timer
    {};
  }
}

#endif // ALGORITHM_NEWTON_PARAMETER_HH
