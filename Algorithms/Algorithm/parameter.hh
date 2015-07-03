#ifndef ALGORITHM_ALGORITHM_PARAMETERS_HH
#define ALGORITHM_ALGORITHM_PARAMETERS_HH

#include "Util/Mixins/mixins.hh"

namespace Algorithm
{
  /**
   * @brief Basic parameters for simple algorithms.
   */
class Parameter : public Mixin::Eps, public Mixin::RelativeAccuracy, public Mixin::Verbosity, public Mixin::MaxSteps {};
}

#endif // ALGORITHM_ALGORITHM_PARAMETERS_HH
