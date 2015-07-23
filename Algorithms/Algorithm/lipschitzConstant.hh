#ifndef ALGORITHM_LIPSCHITZ_CONSTANT_HH
#define ALGORITHM_LIPSCHITZ_CONSTANT_HH

#include "Util/Mixins/eps.hh"

namespace Algorithm
{
  class LipschitzConstant : public Mixin::Eps
  {
  public:
    LipschitzConstant(double initialOmega = 1e-3);

    LipschitzConstant& operator=(double newOmega);

    operator double() const;

    double last() const;

  private:
    double omega_ = 1e-3, oldOmega_ = 1e-3, maxFactor_ = 1e6, decreaseFactor_ = 1e-1;
  };
}

#endif // ALGORITHM_LIPSCHITZ_CONSTANT_HH
