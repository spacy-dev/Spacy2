#include "newtonParameter.hh"

namespace Algorithm
{
  void NewtonParameter::setThetaAim(double thetaAim) noexcept
  {
    thetaAim_ = thetaAim;
  }

  void NewtonParameter::setThetaMax(double thetaMax) noexcept
  {
    thetaMax_ = thetaMax;
  }

  double NewtonParameter::thetaAim() const noexcept
  {
    return thetaAim_;
  }

  double NewtonParameter::thetaMax() const noexcept
  {
    return thetaMax_;
  }
}
