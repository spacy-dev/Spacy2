// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "l2Product.hh"

#include "vector.hh"
#include "Spacy/Util/cast.hh"

#include "Spacy/Spaces/RealSpace/real.hh"
#include "Spacy/vector.hh"
#include "Spacy/vectorSpace.hh"

namespace Spacy
{
  namespace FEniCS
  {
    Real l2Product::operator()(const ::Spacy::Vector& x, const ::Spacy::Vector& y) const
    {
      checkSpaceCompatibility(x.space(),y.space());
      return cast_ref<Vector>(x).get().inner( cast_ref<Vector>(y).get() );
    }
  }
}
