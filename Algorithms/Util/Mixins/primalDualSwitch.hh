#ifndef ALGORITHM_UTIL_MIXIN_PRIMAL_DUAL_SWITCH_HH
#define ALGORITHM_UTIL_MIXIN_PRIMAL_DUAL_SWITCH_HH

namespace Algorithm
{
  namespace Interface { class AbstractFunctionSpaceElement; }
  class FunctionSpaceElement;

  namespace Mixin
  {
    class PrimalDualSwitch
    {
    public:
      void reset() const;

      void reset(const PrimalDualSwitch& y) const;

      void disablePrimal() const;

      void disableDual() const;

      bool isPrimalEnabled() const;

      bool isDualEnabled() const;

    private:
      mutable bool disablePrimal_ = false;
      mutable bool disableDual_ = false;
    };
  }


  bool isPrimalDualSwitch(const Interface::AbstractFunctionSpaceElement& x);

  Mixin::PrimalDualSwitch& toPrimalDualSwitch(Interface::AbstractFunctionSpaceElement& x);

  const Mixin::PrimalDualSwitch& toPrimalDualSwitch(const Interface::AbstractFunctionSpaceElement& x);


  Interface::AbstractFunctionSpaceElement& primal(Interface::AbstractFunctionSpaceElement& x);

  const Interface::AbstractFunctionSpaceElement& primal(const Interface::AbstractFunctionSpaceElement& x);

  Interface::AbstractFunctionSpaceElement& dual(Interface::AbstractFunctionSpaceElement& x);

  const Interface::AbstractFunctionSpaceElement& dual(const Interface::AbstractFunctionSpaceElement& x);


  FunctionSpaceElement& primal(FunctionSpaceElement& x);

  const FunctionSpaceElement& primal(const FunctionSpaceElement& x);

  FunctionSpaceElement& dual(FunctionSpaceElement& x);

  const FunctionSpaceElement& dual(const FunctionSpaceElement& x);
}

#endif // ALGORITHM_UTIL_MIXIN_PRIMAL_DUAL_SWITCH_HH
