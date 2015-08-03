#ifndef ALGORITHM_UTIL_MIXIN_PRIMAL_DUAL_SWITCH_HH
#define ALGORITHM_UTIL_MIXIN_PRIMAL_DUAL_SWITCH_HH

#include "Util/castTo.hh"
#include "primalDualSwitch.hh"

namespace Algorithm
{
  namespace Interface { class AbstractFunctionSpaceElement; }
  class FunctionSpaceElement;

  namespace Mixin
  {
    /**
     * @brief Mixin class for separating primal and dual variables.
     */
    class PrimalDualSwitch
    {
    public:
      /**
       * @brief Enable operations on primal and dual operations.
       */
      void reset() const;

      /**
       * @brief Enable operations on primal and dual operations for this object and y.
       */
      void reset(const PrimalDualSwitch& y) const;

      /**
       * @brief Disables operations on primal variables.
       */
      void disablePrimal() const;

      /**
       * @brief Enables operations on primal variables.
       */
      void disableDual() const;

      /**
       * @brief Checks if operations on primal variables are enabled.
       */
      bool isPrimalEnabled() const;

      /**
       * @brief Checks if operations on primal variables are enabled.
       */
      bool isDualEnabled() const;

      /**
       * @brief Disable reset.
       */
      void disableReset() const;

      /**
       * @brief Enable reset.
       */
      void enableReset() const;

    private:
      mutable bool disablePrimal_ = false;
      mutable bool disableDual_ = false;
      mutable bool disableReset_ = false;
    };
  }

  template <class F, class Arg>
  void primalDualIgnoreReset(F&& f, Arg&& x)
  {
    if( is<Mixin::PrimalDualSwitch>(x) ) castTo<Mixin::PrimalDualSwitch>(x).disableReset();
    f(x);
    if( is<Mixin::PrimalDualSwitch>(x) )
    {
      castTo<Mixin::PrimalDualSwitch>(x).enableReset();
      castTo<Mixin::PrimalDualSwitch>(x).reset();
    }
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
