#include "norm.hh"

#include "vectorSpace.hh"
#include "vector.hh"
#include "Spacy/Spaces/RealSpace/real.hh"
#include "Spacy/Util/cast.hh"
#include "Spacy/norm.hh"
#include "Spacy/vector.hh"

#include <cmath>

namespace Spacy
{
  namespace ProductSpace
  {
    Real Norm::operator()(const ::Spacy::Vector& x) const
    {
      const auto& x_ = cast_ref<Vector>(x);

      auto result = Real{0.};
      for( auto i=0u; i< x_.numberOfVariables(); ++i )
      {
        auto nx = norm( x_.variable(i) );
        result += nx * nx;
      }
      return sqrt(result);
    }
  }
}
