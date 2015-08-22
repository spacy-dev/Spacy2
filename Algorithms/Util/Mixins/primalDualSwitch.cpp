#include "primalDualSwitch.hh"

#include "FunctionSpaces/ProductSpace/productSpaceElement.hh"

namespace Algorithm
{
  namespace Mixin
  {
    void PrimalDualSwitch::reset() const
    {
      if( disableReset_ ) return;
      disablePrimal_ = disableDual_ = false;
    }


    void PrimalDualSwitch::reset(const PrimalDualSwitch& y) const
    {
      if( disableReset_ ) return;
      reset();
      y.reset();
    }

    void PrimalDualSwitch::disablePrimal() const
    {
      disablePrimal_ = true;
    }

    void PrimalDualSwitch::disableDual() const
    {
      disableDual_ = true;
    }

    bool PrimalDualSwitch::isPrimalEnabled() const
    {
      return !disablePrimal_;
    }

    bool PrimalDualSwitch::isDualEnabled() const
    {
      return !disableDual_;
    }

    void PrimalDualSwitch::disableReset() const
    {
      disableReset_ = true;
    }

    void PrimalDualSwitch::enableReset() const
    {
      disableReset_ = false;
    }
  }


  Vector& primal(Vector &x)
  {
    castAny<ProductSpaceElement>(x).disableDual();
    return x;
  }

  const Vector& primal(const Vector &x)
  {
    castAny<ProductSpaceElement>(x).disableDual();
    return x;
  }

  Vector& dual(Vector &x)
  {
    castAny<ProductSpaceElement>(x).disablePrimal();
    return x;
  }

  const Vector& dual(const Vector &x)
  {
    castAny<ProductSpaceElement>(x).disablePrimal();
    return x;
  }
}
