#include "primalDualSwitch.hh"

#include "vector.hh"
#include "Util/Exceptions/invalidArgumentException.hh"

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


  Interface::AbstractVector& primal(Interface::AbstractVector &x)
  {
    castTo<Mixin::PrimalDualSwitch>(x).disableDual();
    return x;
  }

  const Interface::AbstractVector& primal(const Interface::AbstractVector &x)
  {
    castTo<Mixin::PrimalDualSwitch>(x).disableDual();
    return x;
  }

  Interface::AbstractVector& dual(Interface::AbstractVector &x)
  {
    castTo<Mixin::PrimalDualSwitch>(x).disablePrimal();
    return x;
  }

  const Interface::AbstractVector& dual(const Interface::AbstractVector &x)
  {
    castTo<Mixin::PrimalDualSwitch>(x).disablePrimal();
    return x;
  }


  Vector& primal(Vector &x)
  {
    primal(x.impl());
    return x;
  }

  const Vector& primal(const Vector &x)
  {
    primal(x.impl());
    return x;
  }

  Vector& dual(Vector &x)
  {
    dual(x.impl());
    return x;
  }

  const Vector& dual(const Vector &x)
  {
    dual(x.impl());
    return x;
  }
}
