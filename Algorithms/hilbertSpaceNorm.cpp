#include "hilbertSpaceNorm.hh"

namespace Algorithm
{
  HilbertSpaceNorm::HilbertSpaceNorm(std::shared_ptr<Interface::HilbertSpaceNorm> implementation)
    : Norm(std::static_pointer_cast<Interface::AbstractNorm>(implementation))
  {}

  double HilbertSpaceNorm::scalarProduct(const FunctionSpaceElement& x, const FunctionSpaceElement& y) const
  {
    return dynamic_cast<const Interface::HilbertSpaceNorm&>(impl()).scalarProduct( x.impl() , y.impl() );
  }

  double HilbertSpaceNorm::squared(const FunctionSpaceElement& x) const
  {
    return scalarProduct(x,x);
  }
}
