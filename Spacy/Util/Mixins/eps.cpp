// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "eps.hh"

#include <cmath>

namespace Spacy
{
  namespace Mixin
  {

    Eps::Eps(double eps) noexcept
      : eps_(eps)
    {}

    void Eps::setEps(double eps)
    {
      eps_ = eps;
      notify();
    }

    double Eps::eps() const noexcept
    {
      return eps_;
    }

    double Eps::sqrtEps() const noexcept
    {
      return sqrt(eps_);
    }

    double Eps::cbrtEps() const noexcept
    {
      return cbrt(eps_);
    }

    void Eps::update(Eps* changedSubject)
    {
      setEps( changedSubject->eps() );
    }
  }
}
