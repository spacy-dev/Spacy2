#include "scalarProduct.hh"

#include "vectorSpace.hh"
#include "vector.hh"
#include "Spacy/Spaces/RealSpace/vectorSpace.hh"
#include "Spacy/Spaces/RealSpace/real.hh"
#include "Spacy/Util/cast.hh"
#include "Spacy/vectorSpace.hh"
#include "Spacy/vector.hh"

namespace Spacy
{
  namespace ProductSpace
  {
    Real ScalarProduct::operator()(const ::Spacy::Vector& x, const ::Spacy::Vector& y) const
    {
      checkSpaceCompatibility(x.space(),y.space());
      const auto& x_ = cast_ref<Vector>(x);
      const auto& y_ = cast_ref<Vector>(y);

      auto result = Real{0.};
      for( auto i=0u; i< x_.numberOfVariables(); ++i )
        result += x_.component(i) * y_.component(i);
      return result;
    }
  }
}
