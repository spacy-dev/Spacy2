#include "hilbertSpaceNorm.hh"

#include <cmath>

namespace Algorithm
{
  HilbertSpaceNorm::HilbertSpaceNorm(std::shared_ptr<Interface::AbstractScalarProduct> implementation)
    : sp_(implementation)
  {}

//  HilbertSpaceNorm::HilbertSpaceNorm(const ScalarProduct& sp)
//    : HilbertSpaceNorm( std::make_shared<Interface::HilbertSpaceNorm>(sp.sharedImpl()) )
//  {}

  double HilbertSpaceNorm::scalarProduct(const Vector& x, const Vector& y) const
  {
    return (*sp_)( x.impl() , y.impl() );
  }

  double HilbertSpaceNorm::squared(const Vector& x) const
  {
    return scalarProduct(x,x);
  }

  double HilbertSpaceNorm::operator()(const Vector& x) const
  {
    return sqrt(squared(x));
  }
}
