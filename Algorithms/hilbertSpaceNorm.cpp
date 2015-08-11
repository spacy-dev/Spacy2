#include "hilbertSpaceNorm.hh"

namespace Algorithm
{
  HilbertSpaceNorm::HilbertSpaceNorm(std::shared_ptr<Interface::HilbertSpaceNorm> implementation)
    : Norm(std::static_pointer_cast<Interface::AbstractNorm>(implementation))
  {}

  HilbertSpaceNorm::HilbertSpaceNorm(const ScalarProduct& sp)
    : HilbertSpaceNorm( std::make_shared<Interface::HilbertSpaceNorm>(sp.sharedImpl()) )
  {}

  double HilbertSpaceNorm::scalarProduct(const Vector& x, const Vector& y) const
  {
    return dynamic_cast<const Interface::HilbertSpaceNorm&>(impl()).scalarProduct( x.impl() , y.impl() );
  }

  double HilbertSpaceNorm::squared(const Vector& x) const
  {
    return scalarProduct(x,x);
  }

  Interface::HilbertSpaceNorm& HilbertSpaceNorm::impl()
  {
    return dynamic_cast<Interface::HilbertSpaceNorm&>(Norm::impl());
  }

  const Interface::HilbertSpaceNorm& HilbertSpaceNorm::impl() const
  {
    return dynamic_cast<const Interface::HilbertSpaceNorm&>(Norm::impl());
  }

  std::shared_ptr<Interface::HilbertSpaceNorm> HilbertSpaceNorm::sharedImpl() const
  {
    return std::static_pointer_cast<Interface::HilbertSpaceNorm>(Norm::sharedImpl());
  }
}
