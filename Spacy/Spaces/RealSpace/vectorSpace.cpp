#include "vectorSpace.hh"

#include "Spacy/Util/cast.hh"
#include "Spacy/Spaces/RealSpace/real.hh"
#include "Spacy/vector.hh"

Spacy::VectorSpace Spacy::RealSpace::makeHilbertSpace()
{
  return ::Spacy::makeHilbertSpace( [](const VectorSpace* space)
                                      {
                                        return Real{*space};
                                      } ,
                                    [](const ::Spacy::Vector& x, const ::Spacy::Vector& y)
                                      {
                                        return Real( toDouble( cast_ref<Real>(x) * cast_ref<Real>(y) ) );
                                      }
                                   );
}
