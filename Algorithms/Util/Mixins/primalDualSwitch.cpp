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


  bool isPrimalDualSwitch(const Interface::AbstractFunctionSpaceElement& x)
  {
    return dynamic_cast<const Mixin::PrimalDualSwitch*>(&x) != nullptr;
  }

  Mixin::PrimalDualSwitch& toPrimalDualSwitch(Interface::AbstractFunctionSpaceElement& x)
  {
    if( !isPrimalDualSwitch(x) ) throw InvalidArgumentException("toPrimalDualSwitch(Interface::AbstractFunctionSpaceElement&)");
    return dynamic_cast<Mixin::PrimalDualSwitch&>(x);
  }

  const Mixin::PrimalDualSwitch& toPrimalDualSwitch(const Interface::AbstractFunctionSpaceElement& x)
  {
    if( !isPrimalDualSwitch(x) ) throw InvalidArgumentException("toPrimalDualSwitch(Interface::AbstractFunctionSpaceElement&)");
    return dynamic_cast<const Mixin::PrimalDualSwitch&>(x);
  }


  Interface::AbstractFunctionSpaceElement& primal(Interface::AbstractFunctionSpaceElement &x)
  {
    toPrimalDualSwitch(x).disableDual();
    return x;
  }

  const Interface::AbstractFunctionSpaceElement& primal(const Interface::AbstractFunctionSpaceElement &x)
  {
    toPrimalDualSwitch(x).disableDual();
    return x;
  }

  Interface::AbstractFunctionSpaceElement& dual(Interface::AbstractFunctionSpaceElement &x)
  {
    toPrimalDualSwitch(x).disablePrimal();
    return x;
  }

  const Interface::AbstractFunctionSpaceElement& dual(const Interface::AbstractFunctionSpaceElement &x)
  {
    toPrimalDualSwitch(x).disablePrimal();
    return x;
  }


  FunctionSpaceElement& primal(FunctionSpaceElement &x)
  {
    primal(x.impl());
    //    toPrimalDualSwitch(x.impl()).disableDual();
    return x;
  }

  const FunctionSpaceElement& primal(const FunctionSpaceElement &x)
  {
    primal(x.impl());
//    toPrimalDualSwitch(x.impl()).disableDual();
    return x;
  }

  FunctionSpaceElement& dual(FunctionSpaceElement &x)
  {
    dual(x.impl());
//    toPrimalDualSwitch(x.impl()).disablePrimal();
    return x;
  }

  const FunctionSpaceElement& dual(const FunctionSpaceElement &x)
  {
    dual(x.impl());
//    toPrimalDualSwitch(x.impl()).disablePrimal();
    return x;
  }
}
