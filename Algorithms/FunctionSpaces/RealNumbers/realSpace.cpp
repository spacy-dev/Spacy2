#include "realSpace.hh"

#include "real.hh"
#include "realProduct.hh"

namespace Algorithm
{
  RealSpace::RealSpace() : Interface::AbstractVectorSpace(std::make_shared<RealProduct>())
  {}

  std::unique_ptr<Interface::AbstractVector> RealSpace::elementImpl() const
  {
    return std::make_unique<Real>(*this);
  }
}
