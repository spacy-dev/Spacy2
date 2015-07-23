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
      disablePrimal_ = disableDual_ = false;
    }


    void PrimalDualSwitch::reset(const PrimalDualSwitch& y) const
    {
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



  FunctionSpaceElement& primal(FunctionSpaceElement &x)
  {
    toPrimalDualSwitch(x.impl()).disableDual();
    return x;
  }

  const FunctionSpaceElement& primal(const FunctionSpaceElement &x)
  {
    toPrimalDualSwitch(x.impl()).disableDual();
    return x;
  }

  FunctionSpaceElement& dual(FunctionSpaceElement &x)
  {
    toPrimalDualSwitch(x.impl()).disablePrimal();
    return x;
  }

  const FunctionSpaceElement& dual(const FunctionSpaceElement &x)
  {
    toPrimalDualSwitch(x.impl()).disablePrimal();
    return x;
  }
}
