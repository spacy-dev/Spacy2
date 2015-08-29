#include "primalDualSwitch.hh"

#include "Algorithms/VectorSpaces/ProductSpace/vector.hh"

namespace Algorithm
{
  namespace Mixin
  {
    PrimalDualSwitch::PrimalDualSwitch()
      : disablePrimal_(false), disableDual_(false), disableReset_(false)
    {}

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
    cast_ref<ProductSpace::Vector>(x).disableDual();
    return x;
  }

  const Vector& primal(const Vector &x)
  {
    cast_ref<ProductSpace::Vector>(x).disableDual();
    return x;
  }

  Vector& dual(Vector &x)
  {
    cast_ref<ProductSpace::Vector>(x).disablePrimal();
    return x;
  }

  const Vector& dual(const Vector &x)
  {
    cast_ref<ProductSpace::Vector>(x).disablePrimal();
    return x;
  }
}
