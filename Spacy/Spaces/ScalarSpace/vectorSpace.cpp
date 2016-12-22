#include "vectorSpace.hh"

#include <Spacy/vector.hh>
#include <Spacy/zeroVectorCreator.hh>
#include <Spacy/Util/cast.hh>
#include "Real.hh"

Spacy::VectorSpace Spacy::Scalar::makeScalarSpace(bool defaultIndex)
{
  return ::Spacy::makeHilbertSpace( [](const VectorSpace* space)
                                      {
                                        return Real{*space};
                                      } ,
                                    [](const ::Spacy::Vector& x, const ::Spacy::Vector& y)
                                      {
                                        return Real( get( cast_ref<Real>(x) * cast_ref<Real>(y) ) );
                                      } ,
                                    defaultIndex
                                   );
}

Spacy::VectorSpace Spacy::makeScalarSpace(bool defaultIndex)
{
  return Spacy::Scalar::makeScalarSpace(defaultIndex);
}
