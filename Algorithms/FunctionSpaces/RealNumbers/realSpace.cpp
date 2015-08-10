#include "realSpace.hh"

#include "real.hh"
#include "realProduct.hh"

namespace Algorithm
{
  RealSpace::RealSpace() : Interface::AbstractFunctionSpace(std::make_shared<RealProduct>())
  {}

  std::unique_ptr<Interface::AbstractFunctionSpaceElement> RealSpace::elementImpl() const
  {
    return std::make_unique<Real>(*this);
  }
}
