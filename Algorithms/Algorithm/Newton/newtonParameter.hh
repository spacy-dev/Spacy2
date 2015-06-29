#ifndef NEWTONPARAMETER_HH
#define NEWTONPARAMETER_HH

#include "Algorithm/parameter.hh"
#include "Algorithm/regularityTest.hh"

namespace Algorithm
{
  class NewtonParameter : public Parameter, public RegularityTest
  {
  public:
    void setThetaAim(double) noexcept;
    void setThetaMax(double) noexcept;

    double thetaAim() const noexcept;
    double thetaMax() const noexcept;

  private:
    double thetaMax_ = 0.75, thetaAim_ = 0.5;
  };
}

#endif // NEWTONPARAMETER_HH
