#include "hilbertSpaceNorm.hh"

#include "Interface/abstractScalarProduct.hh"
#include "Interface/abstractVector.hh"

#include <cmath>

namespace Algorithm
{
  namespace Interface
  {
    HilbertSpaceNorm::HilbertSpaceNorm(std::shared_ptr<AbstractScalarProduct> sp)
      : Mixin::SharedImpl<AbstractScalarProduct>(sp)
    {}

    double HilbertSpaceNorm::operator()(const AbstractVector& x) const
    {
      return sqrt(impl()(x,x));
    }

    double HilbertSpaceNorm::squared(const AbstractVector &x) const
    {
      return impl()(x,x);
    }

    double HilbertSpaceNorm::scalarProduct(const AbstractVector &x, const AbstractVector &y) const
    {
      return impl()(x,y);
    }
  }
}
