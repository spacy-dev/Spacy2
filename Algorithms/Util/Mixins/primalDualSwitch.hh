#ifndef ALGORITHM_UTIL_MIXIN_PRIMAL_DUAL_SWITCH_HH
#define ALGORITHM_UTIL_MIXIN_PRIMAL_DUAL_SWITCH_HH

#include "vector.hh"
#include "Util/cast.hh"

namespace Algorithm
{
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
       * @brief Disables the next operation on primal variables.
       */
      void disablePrimal() const;

      /**
       * @brief Disables the next operation on dual variables.
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

  /**
   * @brief Apply f(x) without resetting primal/dual flags in intermediate computations.
   *
   * After application of f(x), x.reset() is called. Use this if you need to maintain the restriction to
   * primal or dual variables for several operations.
   */
  template <class F>
  void primalDualIgnoreReset(F&& f, const Vector& x)
  {
    if( isAny<Mixin::PrimalDualSwitch>(x) ) cast_ref<Mixin::PrimalDualSwitch>(x).disableReset();
    f(x);
    if( isAny<Mixin::PrimalDualSwitch>(x) )
    {
      cast_ref<Mixin::PrimalDualSwitch>(x).enableReset();
      cast_ref<Mixin::PrimalDualSwitch>(x).reset();
    }
  }


  Vector& primal(Vector& x);

  const Vector& primal(const Vector& x);

  Vector& dual(Vector& x);

  const Vector& dual(const Vector& x);
}

#endif // ALGORITHM_UTIL_MIXIN_PRIMAL_DUAL_SWITCH_HH
