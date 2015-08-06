#include "primalDualSwitch.hh"

#include "functionSpaceElement.hh"
#include "Interface/abstractFunctionSpaceElement.hh"
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


  Interface::AbstractFunctionSpaceElement& primal(Interface::AbstractFunctionSpaceElement &x)
  {
    castTo<Mixin::PrimalDualSwitch>(x).disableDual();
    return x;
  }

  const Interface::AbstractFunctionSpaceElement& primal(const Interface::AbstractFunctionSpaceElement &x)
  {
    castTo<Mixin::PrimalDualSwitch>(x).disableDual();
    return x;
  }

  Interface::AbstractFunctionSpaceElement& dual(Interface::AbstractFunctionSpaceElement &x)
  {
    castTo<Mixin::PrimalDualSwitch>(x).disablePrimal();
    return x;
  }

  const Interface::AbstractFunctionSpaceElement& dual(const Interface::AbstractFunctionSpaceElement &x)
  {
    castTo<Mixin::PrimalDualSwitch>(x).disablePrimal();
    return x;
  }


  FunctionSpaceElement& primal(FunctionSpaceElement &x)
  {
    primal(x.impl());
    return x;
  }

  const FunctionSpaceElement& primal(const FunctionSpaceElement &x)
  {
    primal(x.impl());
    return x;
  }

  FunctionSpaceElement& dual(FunctionSpaceElement &x)
  {
    dual(x.impl());
    return x;
  }

  const FunctionSpaceElement& dual(const FunctionSpaceElement &x)
  {
    dual(x.impl());
    return x;
  }
}
