#include "hilbertSpaceNorm.hh"

#include "Interface/abstractScalarProduct.hh"
#include "Interface/abstractFunctionSpaceElement.hh"

#include <cmath>

namespace Algorithm
{
  namespace Interface
  {
    HilbertSpaceNorm::HilbertSpaceNorm(std::shared_ptr<AbstractScalarProduct> sp)
      : Mixin::SharedImpl<AbstractScalarProduct>(sp)
    {}

    double HilbertSpaceNorm::operator()(const AbstractFunctionSpaceElement& x) const
    {
      return sqrt(impl()(x,x));
    }

    double HilbertSpaceNorm::squared(const AbstractFunctionSpaceElement &x) const
    {
      return impl()(x,x);
    }

    double HilbertSpaceNorm::scalarProduct(const AbstractFunctionSpaceElement &x, const AbstractFunctionSpaceElement &y) const
    {
      return impl()(x,y);
    }
  }
}
