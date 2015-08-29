#include "dampingAccuracy.hh"

namespace Algorithm
{
  namespace Mixin
  {
    DampingAccuracy::DampingAccuracy(double accuracy) noexcept
      : dampingAccuracy_(accuracy)
    {}

    void DampingAccuracy::setDampingAccuracy(double accuracy) noexcept
    {
      dampingAccuracy_ = accuracy;
    }

    double DampingAccuracy::dampingAccuracy() const noexcept
    {
      return dampingAccuracy_;
    }
  }
}
