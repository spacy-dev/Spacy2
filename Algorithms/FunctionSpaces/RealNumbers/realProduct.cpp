#include "realProduct.hh"

#include "real.hh"
#include "Util/Exceptions/incompatibleSpaceException.hh"
#include "Util/Exceptions/invalidArgumentException.hh"

namespace Algorithm
{
  using Interface::AbstractFunctionSpaceElement;

  double RealProduct::operator()(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& y) const
  {
    if( x.spaceIndex() != y.spaceIndex() ) throw IncompatibleSpaceException("RealProduct",x.spaceIndex(),y.spaceIndex());
    if( dynamic_cast<const Real*>(&x) == nullptr || dynamic_cast<const Real*>(&y) == nullptr ) throw InvalidArgumentException("RealProduct");

    return x.coefficient(0) * y.coefficient(0);
  }
}
