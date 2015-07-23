#include "lipschitzConstant.hh"

#include <algorithm>

namespace Algorithm
{
  LipschitzConstant::LipschitzConstant(double initialOmega)
    : omega_(initialOmega)
  {}

  LipschitzConstant& LipschitzConstant::operator=(double newOmega)
  {
    oldOmega_ = omega_;
    if( newOmega < 0 ) omega_ = decreaseFactor_*oldOmega_;
    else omega_ = newOmega;

    omega_ = std::max(omega_,eps());
    omega_ = std::min(omega_,oldOmega_*maxFactor_);
    return *this;
  }

  LipschitzConstant::operator double() const
  {
    return omega_;
  }

  double LipschitzConstant::last() const
  {
    return oldOmega_;
  }
}
