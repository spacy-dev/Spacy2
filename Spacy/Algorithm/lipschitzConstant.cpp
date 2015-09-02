#include "lipschitzConstant.hh"

#include <algorithm>

namespace Spacy
{
  LipschitzConstant::LipschitzConstant(double initialOmega)
    : omega_(initialOmega)
  {}

  LipschitzConstant& LipschitzConstant::operator=(double newOmega)
  {
    previousOmega_ = omega_;
    if( newOmega < 0 ) omega_ = minFactor_*previousOmega_;
    else omega_ = newOmega;

    omega_ = std::max(omega_,eps());
    omega_ = std::min(omega_,previousOmega_*maxFactor_);
    return *this;
  }

  LipschitzConstant::operator double() const
  {
    return omega_;
  }

  double LipschitzConstant::previous() const
  {
    return previousOmega_;
  }

  void LipschitzConstant::setMaxFactor(double factor)
  {
    maxFactor_ = factor;
  }

  void LipschitzConstant::setMinFactor(double factor)
  {
    minFactor_ = factor;
  }
}
