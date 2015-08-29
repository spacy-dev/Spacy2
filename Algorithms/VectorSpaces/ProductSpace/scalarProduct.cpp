#include "scalarProduct.hh"

#include "Algorithms/VectorSpaces/ProductSpace/vectorSpace.hh"
#include "Algorithms/VectorSpaces/ProductSpace/vector.hh"
#include "Algorithms/Util/Exceptions/incompatibleSpaceException.hh"
#include "Algorithms/Util/cast.hh"

namespace Algorithm
{
  namespace ProductSpace
  {
    double ScalarProduct::operator()(const ::Algorithm::Vector& x, const ::Algorithm::Vector& y) const
    {
      if( x.space()->index() != y.space()->index() )
        throw IncompatibleSpaceException("ProductSpace::ScalarProduct",x.space()->index(),y.space()->index());

      const auto& x_ = cast_ref<Vector>(x);
      const auto& y_ = cast_ref<Vector>(y);

      if( x_.isPrimalDual() )
      {
        auto result = 0;
        if( x_.isPrimalEnabled() && y_.isPrimalEnabled() ) result += x_.primalComponent() * y_.primalComponent();
        if( x_.isDualEnabled() && y_.isDualEnabled() ) result += x_.dualComponent() * y_.dualComponent();
        x_.reset(y_);
        return result;
      }

      auto result = 0.;
      for( auto i : x_.creator().primalSubSpaceIds() )
        result += x_.variable(i)( y_.variable(i) );
      return result;
    }
  }
}
