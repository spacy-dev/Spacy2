// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "findGlobalMinimizer.hh"

#include <cassert>

namespace Spacy
{
  Real findGlobalMinimizer(const std::function<Real(const Real)>& f, Real a, Real b, Real eps)
  {
    assert(a<b);
    eps *= b-a;
    Real tmin = a;
    Real fmin = f(a);

    while( (a+=eps) <= b)
    {
      if( f(a) < fmin )
      {
        fmin = f(a);
        tmin = a;
      }
    }

    return tmin;
  }
}
