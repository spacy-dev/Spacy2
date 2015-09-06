#include "scalarProduct.hh"

#include "Spacy/Spaces/ProductSpace/vectorSpace.hh"
#include "Spacy/Spaces/ProductSpace/vector.hh"
#include "Spacy/Spaces/RealSpace/vectorSpace.hh"
#include "Spacy/Spaces/RealSpace/real.hh"
#include "Spacy/Util/Exceptions/incompatibleSpaceException.hh"
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

      if( x_.isPrimalDual() )
      {
        auto result = Real{0.};
        if( x_.isPrimalEnabled() && y_.isPrimalEnabled() ) result += x_.primalComponent() * y_.primalComponent();
        if( x_.isDualEnabled() && y_.isDualEnabled() ) result += x_.dualComponent() * y_.dualComponent();
        x_.reset(y_);
        return result;
      }

      auto result = Real{0.};
      for( auto i : x_.creator().primalSubSpaceIds() )
        result += x_.variable(i) * y_.variable(i);
      return result;
    }
  }
}
