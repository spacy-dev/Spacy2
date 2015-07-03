#ifndef NEWTONPARAMETER_HH
#define NEWTONPARAMETER_HH

#include "Algorithm/parameter.hh"
#include "Algorithm/regularityTest.hh"
#include "Util/Mixins/contractionRate.hh"

namespace Algorithm
{
  class NewtonParameter : public Parameter, public RegularityTest, public Mixin::ContractionRate
  {};
}

#endif // NEWTONPARAMETER_HH
