#include "realSpace.hh"

#include "real.hh"
#include "realProduct.hh"

namespace Algorithm
{
  RealSpace::RealSpace() : AbstractHilbertSpace(std::make_shared<RealProduct>())
  {}

  std::unique_ptr<AbstractFunctionSpaceElement> RealSpace::elementImpl() const
  {
    return std::make_unique<Real>(*this);
  }
}
