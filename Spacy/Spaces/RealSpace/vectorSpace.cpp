// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "vectorSpace.hh"

#include "Spacy/Util/cast.hh"
#include "real.hh"
#include "Spacy/vector.hh"

Spacy::VectorSpace Spacy::RealSpace::makeHilbertSpace(bool defaultIndex)
{
  return ::Spacy::makeHilbertSpace( [](const VectorSpace* space)
                                      {
                                        return Real{*space};
                                      } ,
                                    [](const ::Spacy::Vector& x, const ::Spacy::Vector& y)
                                      {
                                        return Real( toDouble( cast_ref<Real>(x) * cast_ref<Real>(y) ) );
                                      } ,
                                    defaultIndex
                                   );
}

Spacy::VectorSpace Spacy::makeRealSpace(bool defaultIndex)
{
  return Spacy::RealSpace::makeHilbertSpace(defaultIndex);
}
