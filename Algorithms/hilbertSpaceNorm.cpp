#include "hilbertSpaceNorm.hh"

#include "Interface/abstractDualPairing.hh"
#include "Interface/abstractFunctionSpaceElement.hh"

#include <cmath>

namespace Algorithm
{
  HilbertSpaceNorm::HilbertSpaceNorm(std::shared_ptr<AbstractDualPairing> sp)
    : sp_(sp)
  {}

  double HilbertSpaceNorm::operator()(const AbstractFunctionSpaceElement& x) const
  {
    return sqrt((*sp_)(x,x));
  }

  double HilbertSpaceNorm::squared(const AbstractFunctionSpaceElement &x) const
  {
    return (*sp_)(x,x);
  }

  double HilbertSpaceNorm::scalarProduct(const AbstractFunctionSpaceElement &x, const AbstractFunctionSpaceElement &y) const
  {
    return (*sp_)(x,y);
  }
}
