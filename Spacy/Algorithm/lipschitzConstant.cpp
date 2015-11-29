// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "lipschitzConstant.hh"

#include <algorithm>

namespace Spacy
{
  LipschitzConstant::LipschitzConstant(Real initialOmega)
    : omega_(initialOmega)
  {}

  LipschitzConstant& LipschitzConstant::operator=(Real newOmega)
  {
    previousOmega_ = omega_;
    if( newOmega < 0 ) omega_ = minFactor_*previousOmega_;
    else omega_ = newOmega;

    omega_ = max(omega_,eps());
    omega_ = min(omega_,previousOmega_*maxFactor_);
    return *this;
  }

  LipschitzConstant::operator Real() const
  {
    return omega_;
  }

  Real LipschitzConstant::operator()() const
  {
    return omega_;
  }

  Real LipschitzConstant::previous() const
  {
    return previousOmega_;
  }

  void LipschitzConstant::setMaxFactor(Real factor)
  {
    maxFactor_ = factor;
  }

  void LipschitzConstant::setMinFactor(Real factor)
  {
    minFactor_ = factor;
  }
}
