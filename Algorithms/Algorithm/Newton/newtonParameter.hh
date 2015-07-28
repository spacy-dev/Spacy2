#ifndef ALGORITHM_NEWTON_PARAMETER_HH
#define ALGORITHM_NEWTON_PARAMETER_HH

#include "Algorithm/parameter.hh"
#include "Util/Mixins/regularityTest.hh"
#include "Util/Mixins/contractionRate.hh"

namespace Algorithm
{
  namespace Newton
  {
    class NewtonParameter : public Parameter, public Mixin::RegularityTest
    {};
  }
}

#endif // ALGORITHM_NEWTON_PARAMETER_HH
