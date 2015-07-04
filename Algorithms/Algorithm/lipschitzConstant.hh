#ifndef ALGORITHM_LIPSCHITZ_CONSTANT_HH
#define ALGORITHM_LIPSCHITZ_CONSTANT_HH

#include "Util/Mixins/eps.hh"

namespace Algorithm
{
  class LipschitzConstant : public Mixin::Eps
  {
  public:
    LipschitzConstant(double initialOmega = 1.);

    LipschitzConstant& operator=(double newOmega);

    operator double() const;

  private:
    double omega_ = 1., oldOmega_ = 1., maxFactor_ = 1e6, decreaseFactor_ = 1e-1;
  };
}

#endif // ALGORITHM_LIPSCHITZ_CONSTANT_HH
