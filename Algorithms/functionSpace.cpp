#include "functionSpace.hh"

#include "../Interface/abstractBanachSpace.hh"

namespace Algorithm
{
  FunctionSpace::FunctionSpace(std::shared_ptr<AbstractBanachSpace> implementation)
    : impl_(implementation), norm_(impl_->getNorm()), dp_(impl_->getDualPairing())
  {}

//  FunctionSpace::FunctionSpace(const AbstractBanachSpace& implementation)
//    : FunctionSpace(implementation.clone())
//  {}

  void FunctionSpace::setDualPairing(const DualPairing& dp)
  {
    dp_ = dp;
  }

  const DualPairing& FunctionSpace::getDualPairing() const
  {
    return dp_;
  }

  const DualPairing& FunctionSpace::getScalarProduct() const
  {
    return dp_;
  }


  const Norm& FunctionSpace::getNorm() const
  {
    return norm_;
  }

  FunctionSpaceElement FunctionSpace::element() const
  {
    return FunctionSpaceElement(impl_->element());
  }

  unsigned FunctionSpace::index() const
  {
    return impl_->index();
  }

  AbstractBanachSpace const& FunctionSpace::impl() const
  {
    return *impl_;
  }

  void FunctionSpace::setDualSpace(FunctionSpace& Y)
  {
    dualSpace_ = &Y;
  }

  const FunctionSpace* FunctionSpace::getDualSpace() const
  {
    return dualSpace_;
  }

}
