// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "dampingFactor.hh"

#include "Spacy/Spaces/RealSpace/real.hh"

#include <cmath>

namespace Spacy
{
  DampingFactor::DampingFactor(double nu, double eps)
    : Mixin::Eps(eps)
  {
    set(nu);
  }

  DampingFactor& DampingFactor::operator=(double nu)
  {
    set(nu);
    return *this;
  }

  DampingFactor::operator double() const
  {
    return nu_;
  }

  DampingFactor::operator Real() const
  {
    return Real(nu_);
  }

  void DampingFactor::set(double nu)
  {
    nu_ = ( abs(1-nu) < eps() ) ? 1. : nu;
  }

  std::ostream& operator<<(std::ostream& os, DampingFactor nu)
  {
    return os << static_cast<double>(nu);
  }
}
